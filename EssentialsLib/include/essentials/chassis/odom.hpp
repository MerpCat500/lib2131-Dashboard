#pragma once

#include "essentials/chassis/chassis.hpp"
#include "essentials/pose.hpp"

namespace essentials {

struct sensors
{
  TrackingWheel parallel1; // Fwd tracking 1
  TrackingWheel parallel2; // Fwd tracking 2
  TrackingWheel perpendicular1;
  pros::IMU     Inertial;
} odom_sensors;

void setSensors(sensors Odom_Sensors);

Pose getPose(bool radians = false);

void setPose(Pose pose, bool radians = false);

void update();

void init();
} // namespace essentials
