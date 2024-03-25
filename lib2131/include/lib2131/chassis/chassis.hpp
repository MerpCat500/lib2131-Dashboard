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

struct Drivetrain
{
  pros::Motor_Group* leftMotors;
  pros::Motor_Group* rightMotors;
  float trackWidth;
  float wheelDiameter;
  float rpm;
};

class Chassis
{
  PID _v_PID;
  PID _w_PID;

  Drivetrain _Drivetrain;

  Chassis(PID_Controls Linear_PID, PID_Controls Angular_PID,
          Drivetrain drivetrain, odom::_sensors Odom_Sensors)
      : _v_PID(Linear_PID), _w_PID(Angular_PID), _Drivetrain(drivetrain)
  {
    if (Odom_Sensors._TW1 == nullptr)
    {
      Odom_Sensors._TW1 =
          &trackingWheel(_Drivetrain.leftMotors, _Drivetrain.wheelDiameter,
                         _Drivetrain.trackWidth / 2, _Drivetrain.rpm);
    }

    if (Odom_Sensors._TW2 == nullptr)
    {
      Odom_Sensors._TW2 =
          &trackingWheel(_Drivetrain.rightMotors, _Drivetrain.wheelDiameter,
                         _Drivetrain.trackWidth / -2, _Drivetrain.rpm);
    }

    odom::sensors = Odom_Sensors;
  }

  void calibrate()
  {
    _Drivetrain.leftMotors->tare_position();
    _Drivetrain.rightMotors->tare_position();

    odom::init();
  }

  void set_Pose(pose new_Pose) { odom::set_pose(new_Pose); }
  pose get_Pose() { return odom::get_pose(); }

  void set_brake_type(pros::motor_brake_mode_e stop_type)
  {
    _Drivetrain.leftMotors->set_brake_modes(stop_type);
    _Drivetrain.rightMotors->set_brake_modes(stop_type);
  }

  // Turn to Heading
  void TurnTo(angle Target_Theta, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal)
  {
    if (async)
    {
      pros::Task task(
          [&]()
          {
            queueMotion(priority);
            TurnTo(Target_Theta, timeout, 100, false);
          });
      pros::delay(10);
      return;
    }
    else
    {
      queueMotion(priority);
    }

    angle AngleErr(0);
    angle theta(0);

    timer Timer(0);

    double mtrPwr(0);
    double mtrCap(0);

    _w_PID.reset();

    while (Timer.get_time() < timeout && !_w_PID.get_settled() &&
           isMotionQueued())
    {
      pose Pose = get_Pose();
      theta = Pose.get_theta().wrapped();

      AngleErr = Target_Theta - theta;

      mtrPwr = _w_PID.update(AngleErr.get_deg());

      mtrCap = AMS(mtrPwr, mtrPwr, speed);

      _Drivetrain.leftMotors->move_voltage(mtrPwr * mtrCap);
      _Drivetrain.rightMotors->move_voltage(-mtrPwr * mtrCap);
    }
    exitMotion();
  }

  // Face Point
  void TurnTo(double x, double y, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal)
  {
    if (async)
    {
      pros::Task task(
          [&]()
          {
            queueMotion(priority);
            TurnTo(x, y, timeout, 100, false);
          });
      pros::delay(10);
      return;
    }
    else
    {
      queueMotion(priority);
    }

    angle AngleErr(0);
    angle theta(0);
    angle target_theta(0);

    timer Timer(0);

    double mtrPwr(0);
    double mtrCap(0);

    _w_PID.reset();

    while (Timer.get_time() < timeout && !_w_PID.get_settled() &&
           isMotionQueued())
    {
      pose Pose = get_Pose();

      target_theta = angle(atan2(y - Pose.get_y(), x - Pose.get_x()), false);

      theta = Pose.get_theta().wrapped();

      AngleErr = target_theta - theta;

      mtrPwr = _w_PID.update(AngleErr.get_deg());

      mtrCap = AMS(mtrPwr, mtrPwr, speed);

      _Drivetrain.leftMotors->move_voltage(mtrPwr * mtrCap);
      _Drivetrain.rightMotors->move_voltage(-mtrPwr * mtrCap);
    }
    exitMotion();
  }
  // Face Position
  void TurnTo(pose Position, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority = motion_priorities_e::normal)
  {
    if (async)
    {
      pros::Task task(
          [&]()
          {
            queueMotion(priority);
            TurnTo(Position, timeout, 100, false);
          });
      pros::delay(10);
      return;
    }
    else
    {
      queueMotion(priority);
    }

    angle AngleErr(0);
    angle theta(0);
    angle target_theta(0);

    timer Timer(0);

    double mtrPwr(0);
    double mtrCap(0);

    _w_PID.reset();

    while (Timer.get_time() < timeout && !_w_PID.get_settled() &&
           isMotionQueued())
    {
      pose Pose = get_Pose();

      target_theta = angle(atan2(Position.get_y() - Pose.get_y(),
                                 Position.get_x() - Pose.get_x()),
                           false);

      theta = Pose.get_theta().wrapped();

      AngleErr = target_theta - theta;

      mtrPwr = _w_PID.update(AngleErr.get_deg());

      mtrCap = AMS(mtrPwr, mtrPwr, speed);

      _Drivetrain.leftMotors->move_voltage(mtrPwr * mtrCap);
      _Drivetrain.rightMotors->move_voltage(-mtrPwr * mtrCap);
    }
    exitMotion();
  }

  void MoveTo(double Distance, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority =
                  motion_priorities_e::normal);  // Move Distance
  void MoveTo(double x, double y, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority =
                  motion_priorities_e::normal);  // Move to Pnt
  void MoveTo(pose Position, double timeout, int speed = 100,
              bool async = false,
              motion_priorities_e priority =
                  motion_priorities_e::normal);  // Move to Pose

  void stop();
  bool isMoving();
};

}  // namespace lib2131
