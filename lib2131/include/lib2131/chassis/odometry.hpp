#pragma once
#include "lib2131/chassis/pose.hpp"
#include "lib2131/chassis/trackingWheel.hpp"
#include "pros/imu.hpp"
#include "pros/rtos.hpp"

namespace lib2131
{
namespace odom
{

extern pose _current_position;
extern pose last_position;
extern pose _d_current_position;

struct _sensors
{
  trackingWheel* _TW1;   // Parallel to TW2
  trackingWheel* _TW2;   // Parallel to TW1
  trackingWheel* _TW3;   // Perpendicular to TW1 & TW2 (Optional)
  pros::IMU* _Inertial;  // (Used instead of either TW1 or TW2)

  _sensors(trackingWheel* TW1, trackingWheel* TW2, trackingWheel* TW3,
           pros::IMU* Inertial);
};
extern _sensors sensors;

void set_pose(pose new_position);
pose get_pose();
pose get_d_pose();

void init();

}  // namespace odom
}  // namespace lib2131
