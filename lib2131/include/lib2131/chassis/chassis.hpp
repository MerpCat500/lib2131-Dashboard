#pragma once

#include "lib2131/chassis/angle.hpp"
#include "lib2131/chassis/odometry.hpp"
#include "lib2131/chassis/pose.hpp"
#include "lib2131/chassis/trackingWheel.hpp"
#include "lib2131/movement/MotionQ.hpp"
#include "lib2131/movement/pid.hpp"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

namespace lib2131
{
/**
 * ANTI-MOTOR SATURATION
 * @brief Used to stop one motor from moving faster than possible
 * @param  double leftMotors  : LeftMotor Command
 * @param  double rightMotors : RightMotor Command
 * @param  double maxPct      : Motor Speed Cap
 * @return double Output      : Scaler for mV output
 */
double AMS(double leftMotors, double rightMotors, double maxPct)
{
  if (leftMotors > rightMotors)
  {
    return 12000 * (100 / maxPct) / leftMotors;
  }
  else
  {
    return 12000 * (100 / maxPct) / rightMotors;
  }
}
/**
 * @brief Drivetrain : Struct for drivetrain info
 */
struct Drivetrain
{
  /**
   * @param leftMotors : Pointer to left Motor group
   * @param rightMotors : Pointer to right Motor group
   * @param trackWidth : Width of Drive in INCHES. (To wheels not total)
   * @param wheelDiameter : Wheel size. Standard wheels sizes are:
   * 2, 2.75, 3.25, 4
   */

  pros::Motor_Group* leftMotors;
  pros::Motor_Group* rightMotors;
  float trackWidth;
  float wheelDiameter;
  float rpm;
};

class Chassis
{
  PID _v_PID;  // Lateral PID
  PID _w_PID;  // Angular PID

  trackingWheel SubstituteTW1;
  trackingWheel SubstituteTW2;

  Drivetrain _Drivetrain;  // Drivetrain Details
  /**
   * Chassis
   *
   * @param PID_Controls Linear_PID : Control Values for the Linear PID
   * @param PID_Controls Angular_PID : Control Values for the Angular PID
   * @param Drivetrain drivetrain : Drivetrain Info
   * @param _sensors Odom_Sensors : Info about Sensors
   */
  Chassis(PID_Controls Linear_PID, PID_Controls Angular_PID,
          Drivetrain drivetrain, odom::_sensors Odom_Sensors)
      : _v_PID(Linear_PID),
        _w_PID(Angular_PID),
        _Drivetrain(drivetrain),
        SubstituteTW1(
            trackingWheel(_Drivetrain.leftMotors, _Drivetrain.wheelDiameter,
                          _Drivetrain.trackWidth / 2, _Drivetrain.rpm)),
        SubstituteTW2(
            trackingWheel(_Drivetrain.rightMotors, _Drivetrain.wheelDiameter,
                          _Drivetrain.trackWidth / -2, _Drivetrain.rpm))

  {
    if (Odom_Sensors._TW1 == nullptr)
    {
      Odom_Sensors._TW1 = &SubstituteTW1;
    }

    if (Odom_Sensors._TW2 == nullptr)
    {
      Odom_Sensors._TW2 = &SubstituteTW2;
    }

    odom::sensors = Odom_Sensors;
  }
  /**
   * CALIBRATE THE CHASSIS
   * @brief Reset Drive Motors and initialize Odom
   */
  void calibrate()
  {
    _Drivetrain.leftMotors->tare_position();   // Reset left Drive Position
    _Drivetrain.rightMotors->tare_position();  // Reset right Drive Position

    odom::init();  // Calibrate Odom
  }

  /**
   * SET POSITION
   * @brief Set a new position of the robot
   * @param pose new_Pose : Sets the robots pose
   */
  void set_Pose(pose new_Pose) { odom::set_pose(new_Pose); }

  /**
   * GET POSITION
   * @brief Gets robot position as pose
   * @return pose : Robot position
   */
  pose get_Pose() { return odom::get_pose(); }

  /**
   * SET BRAKE TYPES ON DRIVE
   * @brief Sets PROS::BrakeType for each motor
   * @param motor_brake_modes_e : PROS::BrakeType
   */
  void set_brake_type(pros::motor_brake_mode_e stop_type)
  {
    _Drivetrain.leftMotors->set_brake_modes(stop_type);   // set left stop type
    _Drivetrain.rightMotors->set_brake_modes(stop_type);  // set right stop type
  }

  /**
   * TURN TO HEADING
   * @brief Turns to a specified heading
   * @param angle Target_Theta : Target Heading
   * @param double timeout : Max time for motion
   * @param int speed : Speed Cap in %
   * @param bool async : Run in thread
   * @param motion_priorities_e priority : Priority that the motion takes
   */
  void TurnTo(angle Target_Theta, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal)
  {
    if (async)  // Run in own thread?
    {
      // New thread
      pros::Task task([&]() { TurnTo(Target_Theta, timeout, 100, false); });
      pros::delay(10);  // give time for task to init
      return;           // Quit
    }

    int ID = queueMotion(priority);  // Get a motion ID

    // Initialize motion variables
    angle AngleErr(0);
    angle theta(0);

    // Init Motor Variables
    double mtrPwr(0);
    double mtrCap(0);

    // Start a timer at time 0
    timer Timer(0);

    // Reset angular PID
    _w_PID.reset();

    // Run if ...
    // Within timeout
    // PID isn't settled
    // ID is still active
    while (Timer.get_time() < timeout && !_w_PID.get_settled() && checkID(ID))
    {
      pose Pose = get_Pose();              // Get Current Position
      theta = Pose.get_theta().wrapped();  // Get angle [0, 360)

      AngleErr = Target_Theta - theta;  // Calculate angle error

      // Take Shortest distance if error is > 180°
      if (fabs(AngleErr.get_deg()) > 180)
      {
        AngleErr.get_deg() > 0 ? AngleErr -= angle(360)
                               : AngleErr += angle(360);
      }

      mtrPwr = _w_PID.update(AngleErr.get_deg());  // Update get motor output
      mtrCap = AMS(mtrPwr, mtrPwr, speed);         // Get Cap Ratio

      // Spin Motors
      // Anti-Motor Saturate
      _Drivetrain.leftMotors->move_voltage(mtrPwr * mtrCap);
      _Drivetrain.rightMotors->move_voltage(-mtrPwr * mtrCap);
    }
    clearMotions();  // End Motion
  }

  /**
   * TURN TO POINT
   * @brief turns to a specified point
   * @param double x : X Value
   * @param double y : Y Value
   * @param double timeout : Max time for motion
   * @param int speed : Speed Cap in %
   * @param bool async : Run in thread
   * @param motion_priorities_e priority : Priority that the motion takes
   */
  void TurnTo(double x, double y, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal)
  {
    if (async)  // Run in own thread?
    {
      // Start new thread
      pros::Task task([&]() { TurnTo(x, y, timeout, 100, false); });
      pros::delay(10);  // Allow thread to init
      return;           // QUIT
    }

    int ID = queueMotion(priority);  // Get ID

    // Initialize Motion Variables
    angle theta(0);
    angle target_theta(0);
    angle AngleErr(0);

    // Motor Variables
    double mtrPwr(0);
    double mtrCap(0);

    // Init Timer at 0 msec
    timer Timer(0);

    // Reset Angular PID
    _w_PID.reset();

    // Run if...
    // Timer is less than timeout
    // Angular PID not settled
    // ID is still active
    while (Timer.get_time() < timeout && !_w_PID.get_settled() && checkID(ID))
    {
      pose Pose = get_Pose();  // Get Current Position

      // Calculate Target Angle
      target_theta = angle(atan2(y - Pose.get_y(), x - Pose.get_x()), false);

      theta = Pose.get_theta().wrapped();  // Get Current theta [0, 360)
      AngleErr = target_theta - theta;     // Get Angle Error

      mtrPwr = _w_PID.update(AngleErr.get_deg());  // Calculate Motor Power
      mtrCap = AMS(mtrPwr, mtrPwr, speed);         // Get Motor Cap

      // Spin motor
      // Anti Motor Saturation
      _Drivetrain.leftMotors->move_voltage(mtrPwr * mtrCap);
      _Drivetrain.rightMotors->move_voltage(-mtrPwr * mtrCap);
    }
    clearMotions();  // End Motion
  }

  /**
   * TURN TO POSITION
   * @brief turns to a specified pose
   * @param pose Position : Target Position
   * @param double timeout : Max time for motion
   * @param int speed : Speed Cap in %
   * @param bool async : Run in thread
   * @param motion_priorities_e priority : Priority that the motion takes
   */
  void TurnTo(pose Position, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal)
  {
    if (async)  // Run in own thread?
    {
      // Start new thread
      pros::Task task([&]() { TurnTo(Position, timeout, 100, false); });
      pros::delay(10);  // let thread init
      return;           // QUIT
    }

    int ID = queueMotion(priority);  // Get ID

    // Init Motion Variables
    angle AngleErr(0);
    angle theta(0);
    angle target_theta(0);

    // Motor Variables
    double mtrPwr(0);
    double mtrCap(0);

    // Init timer at 0 msec
    timer Timer(0);

    // Reset Angular PID
    _w_PID.reset();

    // Run if...
    // Timer is less than timeout
    // Angular PID isn't settled
    // ID is still active
    while (Timer.get_time() < timeout && !_w_PID.get_settled() && checkID(ID))
    {
      pose Pose = get_Pose();  // Get Current Position

      // Calculate Target theta
      target_theta = angle(atan2(Position.get_y() - Pose.get_y(),
                                 Position.get_x() - Pose.get_x()),
                           false);

      theta = Pose.get_theta().wrapped();  // Get Current angle [0, 360)

      AngleErr = target_theta - theta;  // Calculate Angle Error

      mtrPwr = _w_PID.update(AngleErr.get_deg());  // Calculate Motor Power
      mtrCap = AMS(mtrPwr, mtrPwr, speed);         // Calculate Motor Cap

      // Spin Motors w/
      // Anti Motor Saturation
      _Drivetrain.leftMotors->move_voltage(mtrPwr * mtrCap);
      _Drivetrain.rightMotors->move_voltage(-mtrPwr * mtrCap);
    }
    clearMotions();  // End Motion
  }

  /**
   * MOVE LINEAR DISTANCE
   * @brief Move a Distance (- for backwards)
   * @param  double target_distance : Distance Traveled
   * @param double timeout : Time alloted to motion
   * @param int speed : Max speed in %
   * @param bool async : Run in own thread
   * @param motion_priorities_e priority : Priority of Motion
   */
  void MoveTo(double target_distance, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal)
  {
    if (async)  // Run in own thread
    {
      // Start Thread
      pros::Task task([&]() { MoveTo(target_distance, timeout, 100, false); });
      pros::delay(10);  // Let thread init
      return;           // Quit
    }

    int ID = queueMotion(priority);  // Get ID

    // Initialize Motion Variables
    double current_distance(0);
    double distance_error(0);

    // Motor Variables
    double mtrPwr(0);
    double mtrCap(0);

    // Init Timer at 0 msec
    timer Timer(0);

    // Reset Tracking Wheels
    odom::sensors._TW1->reset();
    odom::sensors._TW2->reset();

    // Reset Linear PID
    _v_PID.reset();

    // Run if...
    // Timer less than timeout
    // Linear PID not settled
    // ID still active
    while (Timer.get_time() < timeout && !_v_PID.get_settled() && checkID(ID))
    {
      // Avg Current Position
      current_distance =
          (odom::sensors._TW1->get_Dist() + odom::sensors._TW2->get_Dist()) / 2;
      // Calculate Error
      distance_error = target_distance - current_distance;

      mtrPwr = _v_PID.update(distance_error);  // Calculate Motor Power
      mtrCap = AMS(mtrPwr, mtrPwr, speed);     // Calculate Cap

      // Spin mtr w/
      // Anti Motor Saturation
      _Drivetrain.leftMotors->move_voltage(mtrPwr * mtrCap);
      _Drivetrain.rightMotors->move_voltage(mtrPwr * mtrCap);
    }
  }

  /**
   *
   * @param double x : X Value
   * @param double y : Y Value
   * @param double timeout : Time alloted to motion
   * @param int speed : Max speed in %
   * @param bool async : Run in own thread
   * @param motion_priorities_e priority : Priority of Motion
   */
  void MoveTo(double x, double y, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal)
  {
    if (async)  // Run in own thread?
    {
      // Start new task
      pros::Task task([&]() { MoveTo(x, y, timeout, 100, false); });
      pros::delay(10);  // Let task init
      return;           // Quit
    }

    int ID = queueMotion(priority);  // Get ID

    // Initialize Motion Variables
    angle deltaTheta(0);
    double targetTheta;

    bool forwards(0);

    // Distances
    double distance_V(0);
    angle distance_W(0);

    // Errors
    double error_V(0);
    angle error_W(0);

    // Motors
    double mtrCap(0);

    double lMtrPwr(0);
    double rMtrPwr(0);

    // Init Timer at time 0 msec
    timer Timer(0);

    // Rest linear and angular PID
    _w_PID.reset();
    _v_PID.reset();

    pose Pose = odom::get_pose();  // Get Current Position

    // Decide to drive forwards or backwards
    if (fabs((angle(atan2(x - Pose.get_x(), y - Pose.get_y()), false) +
              Pose.get_theta())
                 .get_deg()) < 90)
    {
      forwards = true;
    }
    else
    {
      forwards = false;
    }

    // Run if...
    // Timer within timeout
    // Angular nor Linear PID settled
    // ID is still active
    while (Timer.get_time() < timeout && !_w_PID.get_settled() &&
           !_v_PID.get_settled() && checkID(ID))
    {
      pose Pose = odom::get_pose();  // Get Current Position

      // Calculate Distance
      distance_V = sqrt(pow(x - Pose.get_x(), 2) + pow(y - Pose.get_y(), 2));
      // Calculate Angle
      distance_W = angle(atan2(x - Pose.get_x(), y - Pose.get_y()), false);

      if (forwards)  // Adjust angle error for driving backwards
      {
        error_W = distance_W;
      }
      else
      {
        error_W = distance_W - angle(M_PI_2, false);
      }

      // Calculate Linear error
      // Scaled by angle
      error_V = distance_V * cos(error_W.get_rad());

      // Output Variables
      double out_V;
      double out_W;

      if (error_V < 6)  // If Close to target then slow
      {
        out_V = _v_PID.update(error_V);  // Calculate Linear Power
        out_W = 0;                       // Don't Turn
      }
      else
      {
        // Calculate Linear Power and slow by 33%
        out_V = _v_PID.update(error_V) * (1 / 3);
        // Calculate Angular Power
        out_W = _w_PID.update(error_W.get_deg());
      }

      lMtrPwr = out_V + out_W;  // Calculate Motor Powers
      rMtrPwr = out_V - out_W;  // Calculate Motor Powers

      mtrCap = AMS(lMtrPwr, rMtrPwr, speed);  // Get Motor Cap

      // Spin motors w/
      // Anti Motor Saturation
      _Drivetrain.leftMotors->move_voltage(lMtrPwr * mtrCap);
      _Drivetrain.rightMotors->move_voltage(rMtrPwr * mtrCap);
    }
  }

  // Move to Pose
  void MoveTo(pose Position, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal);

  void stop();
  bool isMoving();
};

}  // namespace lib2131
