#include "lib2131/chassis/angle.hpp"
namespace lib2131
{
/**
 * Angle
 * @brief Constructor for an Angle
 * @param double value : An input value
 * @param bool isDegrees : is input value in degrees?
 */
angle::angle(double value, bool isDegrees)
{
  if (isDegrees)  // Value is in degrees
  {
    this->_degrees = value;  // Degrees = value
  }
  else
  {
    this->_degrees = value * 180 / M_PI;  // Calculate Degrees
  }
}
/**
 * Get Degrees
 * @brief Returns Angle Value in Degrees
 * @return double : Angle Value in Degrees
 */
double const angle::get_deg() { return _degrees; }
/**
 * Get Radians
 * @brief Returns Angle Value in Radians
 * @return double : Angle Value in Radians
 */
double const angle::get_rad() { return _degrees * M_PI / 180; }
/**
 * Addition
 * @brief Returns the Sum of two angles
 * @param angle B : Added Angle
 * @return angle : Sum of two angles
 */
angle angle::addition(angle B)
{
  return angle(this->_degrees + B.get_deg(), true);
}
/**
 * Subtraction
 * @brief Returns the Difference of two angles
 * @param angle B : Subtracted Angle
 * @return angle : Difference of two angles
 */
angle angle::subtract(angle B)
{
  return angle(this->_degrees - B.get_deg(), true);
}
/**
 * Multiply
 * @brief Scale angle by a Scalar
 * @param double B : Scalar
 * @return angle : Scaled Angle
 */
angle angle::multiply(double B) { return angle(this->_degrees * B, true); }

/**
 * Addition Equal
 * @brief Adds an angle too self
 * @param angle B : Added Angle
 * @return angle : Self
 */
angle angle::additionEqual(angle B)
{
  this->_degrees += B.get_deg();
  return (*this);  // Return Self
}
/**
 * Subtraction Equal
 * @brief Subtracts angles from self
 * @param angle B : Subtracted Angle
 * @return angle : Self
 */
angle angle::subtractEqual(angle B)
{
  this->_degrees -= B.get_deg();
  return (*this);  // Return Self
}
/**
 * Multiply Equal
 * @brief Scale angle by a Scalar
 * @param double B : Scalar
 * @return angle : Self
 */
angle angle::multiplyEqual(double B)
{
  this->_degrees *= B;
  return (*this);  // Return Self
}
/**
 * Wrap angle
 * @brief Wraps an angle from [0, 360) and [0, 6.28...)
 * @return angle : Wrapped Angle
 */
angle angle::wrapped() { return angle(std::fmod(this->_degrees, 360), true); }
/**
 * Plus Operator
 * @brief Addition of angle for more info use 'Addition'
 */
angle angle::operator+(angle B) { return addition(B); }
/**
 * Dash Operator
 * @brief Subtraction of angle for more info use 'Subtraction'
 */
angle angle::operator-(angle B) { return subtract(B); }
/**
 * Star Operator
 * @brief Scaling of angle for more info use 'Multiply'
 */
angle angle::operator*(double B) { return multiply(B); }
/**
 * Plus Equals Operator
 * @brief Addition to angle for more info use 'AdditionEqual'
 */
angle angle::operator+=(angle B) { return additionEqual(B); }
/**
 * Dash Equals Operator
 * @brief Subtraction from angle for more info use 'SubtractionEquals'
 */
angle angle::operator-=(angle B) { return subtractEqual(B); }
/**
 * Star Equals Operator
 * @brief Scaling of angle for more info use 'MultiplyEquals'
 */
angle angle::operator*=(double B) { return multiplyEqual(B); }

}  // namespace lib2131
