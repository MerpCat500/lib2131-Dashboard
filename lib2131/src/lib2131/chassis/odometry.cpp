#include "lib2131/chassis/odometry.hpp"

namespace lib2131
{
namespace odom
{

_sensors sensors = {nullptr, nullptr, nullptr, nullptr};

double _TW1_delta(0);
double _TW2_delta(0);
double _TW3_delta(0);

double _TW1_offset(0);
double _TW2_offset(0);
double _TW3_offset(0);

angle _IMU_last(0, false);
angle _d_heading(0, false);

pose _current_position(0, 0, 0, false);
pose _last_position(0, 0, 0, false);
pose _d_global_XY(0, 0, 0, false);
pose _d_current_position(0, 0, 0, false);

pros::Task *OdomThread;

_sensors::_sensors(lib2131::trackingWheel *TW1, lib2131::trackingWheel *TW2,
                   lib2131::trackingWheel *TW3, pros::IMU *Inertial)
{
  this->_TW1 = TW1;
  this->_TW2 = TW2;
  this->_TW3 = TW3;
  this->_Inertial = Inertial;
}

/**
 * Set Robot's Current Position
 * @param  {pose} new_position : Set the new Robot Position
 */
void set_pose(pose new_position) { _current_position = new_position; }

/**
 * Get Robot's Current Position
 * @return {pose}  : The Current Position of the Robot
 */
pose get_pose() { return _current_position; }

/**
 * Get Robot's Change in Current Position
 * @return {pose}  : The Current Position of the Robot
 */
pose get_d_pose() { return _d_current_position; }

/**
 * Update Odom Position
 */
void update()
{
  // Local Change
  double _d_local_Y = 0;
  double _d_local_X = 0;
  // Get Sensor info (if available)
  if (sensors._TW1 != nullptr)
  {
    sensors._TW1->update();
    _TW1_delta = sensors._TW1->get_d_Dist();
    _d_local_Y = _TW1_delta;
    _TW1_offset = sensors._TW1->get_Offset();
  }
  if (sensors._TW2 != nullptr)
  {
    sensors._TW2->update();
    _TW2_delta = sensors._TW2->get_d_Dist();
    _TW2_offset = sensors._TW2->get_Offset();
  }
  if (sensors._TW3 != nullptr)
  {
    sensors._TW3->update();
    _TW3_delta = sensors._TW3->get_d_Dist();
    _d_local_X = _TW3_delta;
    _TW2_offset = sensors._TW3->get_Offset();
  }

  angle heading = _current_position.get_theta();
  if (sensors._Inertial != nullptr)
  {
    heading = angle(-sensors._Inertial->get_heading(), true);
    _d_heading = heading - _IMU_last;
  }
  else if (sensors._TW1 != nullptr && sensors._TW2 != nullptr)
  {
    _d_heading =
        angle((_TW1_delta - _TW2_delta) / (_TW1_offset - _TW2_offset), false);

    heading += _d_heading;
  }
  _IMU_last = heading;

  angle avgHeading = _current_position.get_theta() - _d_heading * 0.5;

  if (_d_heading.get_rad() == 0)
  {
    _d_local_Y = _TW1_delta;
    _d_local_X = _TW3_delta;
  }
  else
  {
    _d_local_X = 2 * sin(_d_heading.get_rad() * 0.5) *
                 (_TW3_delta / _d_heading.get_rad() + _TW3_offset);
    _d_local_Y = -2 * sin(_d_heading.get_rad() * 0.5) *
                 (_TW1_delta / _d_heading.get_rad() + _TW1_offset);
    // std::cout << _d_local_X << ", " << _d_local_Y << ", " << heading
    //           << std::endl;
  }

  _d_global_XY = pose(_d_local_Y * sin(avgHeading.get_rad()) +
                          _d_local_X * cos(avgHeading.get_rad()),
                      _d_local_Y * -cos(avgHeading.get_rad()) +
                          _d_local_X * sin(avgHeading.get_rad()),
                      _d_heading);

  _current_position += _d_global_XY;
}  // void update()

void init()
{
  OdomThread = new pros::Task(
      [=]
      {
        if (sensors._Inertial != nullptr)
        {
          sensors._Inertial->reset(true);
        }

        while (1)
        {
          // std::cout << sensors._TW1->get_Dist() << ", "
          //           << sensors._TW2->get_Dist() << std::endl;
          update();
          pros::delay(10);
        }
      },
      "ODOMETRY PROCESS");
}

}  // namespace odom

}  // namespace lib2131
