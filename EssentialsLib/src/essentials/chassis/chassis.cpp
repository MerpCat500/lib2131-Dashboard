#include "essentials/chassis/chassis.hpp"

namespace essentials
{
Chassis::Chassis(chassis_param chassis_param, PID linearPID, PID angularPID)
    : drive_info(chassis_param), linearFB(linearPID), angularFB(angularPID)
{
}

void Chassis::calibrate() { essentials::odom::init(); }
void Chassis::setPose(Pose newPosition)
{
  essentials::odom::setPose(newPosition, true);
}

Pose Chassis::getPose() { return essentials::odom::getPose(); }
void Chassis::setPose(Pose newPose)
{
  essentials::odom::setPose(newPose, true);
}

void Chassis::turnTo() {}
void Chassis::moveTo() {}
}  // namespace essentials
