#include "lib2131/chassis/trackingWheel.hpp"
#include "pros/imu.hpp"

namespace lib2131
{
namespace Odom
{
struct _sensors
{
  trackingWheel* TW1 = nullptr;   // Parallel to TW2
  trackingWheel* TW2 = nullptr;   // Parallel to TW1
  trackingWheel* TW3 = nullptr;   // Perpendicular to TW1 & TW2 (Optional)
  pros::IMU* Inertial = nullptr;  // (Used instead of either TW1 or TW2)
} sensors;

}  // namespace Odom
}  // namespace lib2131
