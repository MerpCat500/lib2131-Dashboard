#pragma once

#include <cmath>

#include "essentials/util.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "pros/rotation.hpp"

namespace essentials
{
class TrackingWheel
{
 private:
  pros::ADIEncoder *encoder = nullptr;
  pros::Rotation *rotation = nullptr;
  pros::Motor_Group *motors = nullptr;

  float diameter;
  float distance;
  float rpm;

 public:
  TrackingWheel(pros::ADIEncoder *encoder, float diameter, float distance);
  TrackingWheel(pros::Rotation *encoder, float diameter, float distance);
  TrackingWheel(pros::Motor_Group *motors, float diameter, float distance,
                float rpm);

  void reset();
  float getRaw();
  float getDistanceTraveled();
  float getOffset();
};
}  // namespace essentials