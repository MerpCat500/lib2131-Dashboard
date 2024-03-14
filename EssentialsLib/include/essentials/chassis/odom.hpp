#pragma once

#include <iostream>
#include <stdexcept>

#include "essentials/chassis/trackingWheel.hpp"
#include "essentials/math/pose.hpp"
#include "essentials/math/vector2.hpp"
#include "essentials/util.hpp"
#include "pros/imu.hpp"
#include "pros/misc.hpp"

namespace essentials
{
namespace odom
{
struct tracking_sensors
{
  TrackingWheel* parallel1;       // Fwd tracking 1
  TrackingWheel* parallel2;       // Fwd tracking 2
  TrackingWheel* perpendicular1;  // Horizontal Track
  pros::IMU* inertial;            // Vex Inertial Sensor
  float offset_radians;           // Angle Rotation of wheels

  tracking_sensors(TrackingWheel* Parallel1, TrackingWheel* Parallel2,
                   TrackingWheel* Perpendicular1, pros::IMU* Inertial,
                   float Offset_radians);
};

extern tracking_sensors odom_sensors;

Pose getPose();                         // Get Current Position
Pose getVel();                          // Get Current Velocity
void setPose(Pose pos, bool resetVel);  // Set Current Position

void update();  // Update Position
void init();    // Initialize
}  // namespace odom
}  // namespace essentials
