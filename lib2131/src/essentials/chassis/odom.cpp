#include "essentials/chassis/odom.hpp"

namespace essentials
{
namespace odom
{

bool initalized(0);

Pose posPose(0, 0, 0);    // Current Position
Pose velPose(0, 0, 0);    // Current Velocity
Pose last_pose(0, 0, 0);  // Last Position

// Distances
float Para1Distance;
float Para2Distance;
float Perp1Distance;

// Last
float Para1Last;
float Para2Last;
float Perp1Last;

// Offsets
float Para1Offset;
float Para2Offset;
float Perp1Offset;

tracking_sensors::tracking_sensors(TrackingWheel* Parallel1,
                                   TrackingWheel* Parallel2,
                                   TrackingWheel* Perpendicular1,
                                   pros::IMU* Inertial, float Offset_radians)
    : parallel1(Parallel1),
      parallel2(Parallel2),
      perpendicular1(Perpendicular1),
      inertial(Inertial),
      offset_radians(Offset_radians)
{
}

Pose getPose() { return posPose; }
Pose getVel() { return velPose; }

void setPose(Pose pos, bool resetVel)
{
  posPose = pos;                      // Set new Position
  if (resetVel) velPose = {0, 0, 0};  // reset Velocity
}

void update()
{
  if (initalized)
  {
    // Update Sensors
    odom_sensors.parallel1 != nullptr
        ? Para1Distance = odom_sensors.parallel1->getDistanceTraveled()
        : Para1Distance = 0;
    odom_sensors.parallel2 != nullptr
        ? Para2Distance = odom_sensors.parallel2->getDistanceTraveled()
        : Para2Offset = 0;
    odom_sensors.perpendicular1 != nullptr
        ? Perp1Distance = odom_sensors.perpendicular1->getDistanceTraveled()
        : Perp1Offset = 0;

    // Tracking Wheel Deltas
    float deltaPara1 = Para1Distance - Para1Last;
    float deltaPara2 = Para2Distance - Para2Last;
    float deltaPerp1 = Perp1Distance - Perp1Last;

    float heading = posPose.getRad();

    if (odom_sensors.parallel1 != nullptr && odom_sensors.parallel2 != nullptr)
    {
      // Find Heading Change (Radian), Two Wheel
      heading += (deltaPara1 - deltaPara2) / (Para1Offset - Para2Offset);
    }
    else if (odom_sensors.inertial != nullptr)
    {
      // Find Heading (Not Change) (Degrees), Inertial
      heading = degToRad(odom_sensors.inertial->get_heading());
    }
    else
    {
      // NEED more sensors, Return Error
      throw std::invalid_argument("ODOM: Missing required sensors");
    }

    // Delta Theta (Radians)
    posPose.setTheta(heading, false);
    float deltaTheta = heading - posPose.getRad();

    // Local Changes
    float localX;
    float localY;

    // If angle change is 0
    deltaTheta != 0
        // Calculate Chord Length
        ? localY = 2 * sin(velPose.getRad() / 2) *
                   (deltaPara1 / velPose.getRad() - Para1Offset)
        // Calculate Traveled Distance
        : localY = deltaPara1;

    deltaTheta != 0
        // Calculate Chord Length
        ? localX = 2 * sin(velPose.getRad() / 2) *
                   (deltaPerp1 / velPose.getRad() - Perp1Offset)
        // Calculate Traveled Distance
        : localX = deltaPerp1;

    // Rotate to Global Headings
    posPose.pos += rotatePoint(vector2(localY, localX),
                               posPose.getRad() + odom_sensors.offset_radians,
                               {0, 0}, false);
    // Update Last
    Para1Last = Para1Distance;
    Para2Last = Para2Distance;
    Perp1Last = Perp1Distance;

    last_pose = posPose;
  }
}

void init()
{
  last_pose = Pose(0, 0, 0, false);

  if (odom_sensors.parallel1 != nullptr)
  {
    Para1Offset = odom_sensors.parallel1->getOffset();
    odom_sensors.parallel1->reset();
  }
  if (odom_sensors.parallel2 != nullptr)
  {
    Para2Offset = odom_sensors.parallel2->getOffset();
    odom_sensors.parallel2->reset();
  }
  if (odom_sensors.perpendicular1 != nullptr)
  {
    Perp1Offset = odom_sensors.perpendicular1->getOffset();
    odom_sensors.perpendicular1->reset();
  }
  if (odom_sensors.inertial != nullptr)
  {
    odom_sensors.inertial->reset(true);
    if (odom_sensors.inertial->get_status() ==
        pros::c::imu_status_e::E_IMU_STATUS_ERROR)
    {
      pros::c::controller_rumble(pros::E_CONTROLLER_MASTER, "... --- ...");
    }
  }
  pros::c::controller_rumble(pros::E_CONTROLLER_MASTER, ".");
  initalized = true;
}

void executeHandler()
{
  init();
  while (1)
  {
    update();
    pros::Task::delay(40);
  }
}

pros::Task executeTask(executeHandler, "ODOM");

}  // namespace odom
}  // namespace essentials
