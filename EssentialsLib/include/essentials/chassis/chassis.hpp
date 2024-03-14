#pragma once

#include "essentials/chassis/odom.hpp"
#include "essentials/pid.hpp"
#include "pros/motors.hpp"

namespace essentials
{

struct chassis_param
{
  float rpm;
  float wheelSize;
  float trackSize;
};

class Chassis
{
  // Chassis Parameters
  chassis_param drive_info;

  // Drive Controllers
  PID linearFB;  // PID FeedBack Controllers
  PID angularFB;

 public:
  Chassis(chassis_param chassis_param, PID linearFB, PID angularFB);
  void calibrate();

  void setPose(Pose newPosition);
  Pose getPose();

  void turnTo();
  void moveTo();
};

}  // namespace essentials
