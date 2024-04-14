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
double AMS(double leftMotors, double rightMotors, double maxPct);
/**
 * DRIVETRAIN STRUCT
 * @brief Drivetrain : Struct for drivetrain info
 * @param leftMotors : Pointer to left Motor group
 * @param rightMotors : Pointer to right Motor group
 * @param trackWidth : Width of Drive in INCHES. (To wheels not total)
 * @param wheelDiameter : Wheel size. Standard wheels sizes are:
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
 public:
  /**
   * Chassis
   *
   * @param PID_Controls Linear_PID : Control Values for the Linear PID
   * @param PID_Controls Angular_PID : Control Values for the Angular PID
   * @param Drivetrain drivetrain : Drivetrain Info
   * @param _sensors Odom_Sensors : Info about Sensors
   */
  Chassis(PID_Controls Linear_PID, PID_Controls Angular_PID,
          Drivetrain drivetrain, odom::_sensors Odom_Sensors);
  /**
   * CALIBRATE THE CHASSIS
   * @brief Reset Drive Motors and initialize Odom
   */
  void calibrate();

  /**
   * SET POSITION
   * @brief Set a new position of the robot
   * @param pose new_Pose : Sets the robots pose
   */
  void set_Pose(pose new_Pose);

  /**
   * GET POSITION
   * @brief Gets robot position as pose
   * @return pose : Robot position
   */
  pose get_Pose();

  /**
   * SET BRAKE TYPES ON DRIVE
   * @brief Sets PROS::BrakeType for each motor
   * @param motor_brake_modes_e : PROS::BrakeType
   */
  void set_brake_type(pros::motor_brake_mode_e stop_type);

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
              motion_priorities_e priority = motion_priorities_e::normal);

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
              motion_priorities_e priority = motion_priorities_e::normal);

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
              motion_priorities_e priority = motion_priorities_e::normal);

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
              motion_priorities_e priority = motion_priorities_e::normal);

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
              motion_priorities_e priority = motion_priorities_e::normal);

  // Move to Pose
  void MoveTo(pose Position, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal);

  void stop();
  bool isMoving();
};

}  // namespace lib2131
