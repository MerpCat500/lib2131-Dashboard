#pragma once
#include <cmath>
#include <iostream>

#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "pros/rotation.hpp"

namespace lib2131
{
/**
 * Different Types of Wheels
 * (In Inches)
 * Tiny: 2.00
 * Small: 2.75
 * Medium: 3.25
 * Large: 4.00
 */
namespace WheelTypes
{
constexpr double Tiny_Omni = 2.00;
constexpr double Small_Omni = 2.75;
constexpr double Med_Omni = 3.25;
constexpr double Large_Omni = 4.00;
}  // namespace WheelTypes

class trackingWheel
{
 private:
  pros::ADIEncoder* _Encoder = nullptr;
  pros::Rotation* _Rotational = nullptr;
  pros::Motor_Group* _Motor_Group = nullptr;

  const double _WheelSize;
  const double _Ratio;
  const double _Offset;

  double _Dist_Traveled = 0;
  double _last_Dist_Traveled = 0;
  double _d_Dist_Traveled = 0;

 public:
  /**
   * Constructor for a Tracking Wheel using a ADI (Triport) Encoder
   *
   * @param  pros::ADIEncoder* Encoder : Pointer to sensor
   * @param  double WheelSize          : Diameter of the wheel
   * @param  double Offset             : Offset from tracking center
   * @param  double Ratio              : Ratio from wheel to sensor
   */
  trackingWheel(pros::ADIEncoder* Encoder, double WheelSize, double Offset,
                double Ratio = 1);
  /**
   * Constructor for a Tracking Wheel using a Rotational Sensor
   *
   * @param pros::Rotation* Rotational : Pointer to sensor
   * @param double WheelSize           : Diameter of the Wheel
   * @param double Offset              : Offset from tracking center
   * @param double Ratio               : Ratio from wheel to sensor
   */
  trackingWheel(pros::Rotation* Rotational, double WheelSize, double Offset,
                double Ratio = 1);
  /**
   * Constructor for a Tracking Wheel using a Motor Group
   *
   * @param  pros::Motor_Group* Motor_Group : Pointer to Motor_Group
   * @param  double WheelSize               : Diameter of the Wheel
   * @param  double Offset                  : Drive Width
   * @param  double Ratio                   : Rpm of Drive
   */
  trackingWheel(pros::Motor_Group* Motor_Group, double WheelSize, double Offset,
                double Ratio);
  /**
   * Set the current Distance that a Tracking wheel has moved
   * @param  double Distance
   */
  void set_Dist(double Distance);
  /**
   * Get raw output of sensors in Degrees
   *
   * @return double Position: Position of sensor in Degrees
   */
  double get_Raw();
  /**
   * Get distance the Tracking Wheel has traveled
   * @return {double}  Distance Traveled:
   */
  double get_Dist();
  /**
   * Get the Delta of Distance the Tracking Wheel has traveled
   * @return {double}  Delta Distance Traveled:
   */
  double get_d_Dist();
  /**
   * Get Offset from the Tracking Center
   * @return {double}  : Offset Distance
   */
  double get_Offset();
  /**
   * Returns true if the Tracking wheel isn't a motor
   * @return {bool}  : Is Dead?
   */
  bool get_dead();

  /**
   * Update Tracking Wheel
   */
  void update();
  /**
   * Reset Tracking Wheel
   */
  void reset();
};
}  // namespace lib2131
