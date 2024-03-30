#pragma once
#include <cmath>
#include <iostream>

namespace lib2131
{
/** TODO : Change How this Class works, It really should use just one variable
 * and a boolean if it's in degrees, This costs too much on processor and
 * memory.
 **/

/**
 * ANGLE CLASS
 * @brief A Class for any type of angle. "Degrees, Radians, We got it all :D"
 */
class angle
{
 private:
  // Angle in rad and deg
  double _degrees;

 public:
  /**
   * Angle
   * @brief Constructor for an Angle
   * @param double value : An input value
   * @param bool isDegrees : is input value in degrees?
   */
  angle(double value, bool isDegrees = true);

  /**
   * Get Degrees
   * @brief Returns Angle Value in Degrees
   * @return double : Angle Value in Degrees
   */
  double const get_deg();

  /**
   * Get Radians
   * @brief Returns Angle Value in Radians
   * @return double : Angle Value in Radians
   */
  double const get_rad();

  /**
   * Addition
   * @brief Returns the Sum of two angles
   * @param angle B : Added Angle
   * @return angle : Sum of two angles
   */
  angle addition(angle B);
  /**
   * Subtraction
   * @brief Returns the Difference of two angles
   * @param angle B : Subtracted Angle
   * @return angle : Difference of two angles
   */
  angle subtract(angle B);
  /**
   * Multiply
   * @brief Scale angle by a Scalar
   * @param double B : Scalar
   * @return angle : Scaled Angle
   */
  angle multiply(double B);

  /**
   * Addition Equal
   * @brief Adds an angle too self
   * @param angle B : Added Angle
   * @return angle : Self
   */
  angle additionEqual(angle B);
  /**
   * Subtraction Equal
   * @brief Subtracts angles from self
   * @param angle B : Subtracted Angle
   * @return angle : Self
   */
  angle subtractEqual(angle B);
  /**
   * Multiply Equal
   * @brief Scale angle by a Scalar
   * @param double B : Scalar
   * @return angle : Self
   */
  angle multiplyEqual(double B);

  /**
   * Wrap angle
   * @brief Wraps an angle from [0, 360) and [0, 6.28...)
   * @return angle : Wrapped Angle
   */
  angle wrapped();

  /**
   * Plus Operator
   * @brief Addition of angle for more info use 'Addition'
   */
  angle operator+(angle B);
  /**
   * Dash Operator
   * @brief Subtraction of angle for more info use 'Subtraction'
   */
  angle operator-(angle B);
  /**
   * Star Operator
   * @brief Scaling of angle for more info use 'Multiply'
   */
  angle operator*(double B);

  /**
   * Plus Equals Operator
   * @brief Addition to angle for more info use 'AdditionEqual'
   */
  angle operator+=(angle B);
  /**
   * Dash Equals Operator
   * @brief Subtraction from angle for more info use 'SubtractionEquals'
   */
  angle operator-=(angle B);
  /**
   * Star Equals Operator
   * @brief Scaling of angle for more info use 'MultiplyEquals'
   */
  angle operator*=(double B);

  /**
   * COUT OVERLOAD
   * @brief "Print" a angle to terminal
   * @param  ostream os : Output stream to use (Cout, Cerr, etc)
   * @param  angle B : Reference to angle being used
   * @return ostream : Cout
   */
  friend std::ostream& operator<<(std::ostream& os, angle const& B)
  {
    // Print <Degrees> + '°, ' + <Radians> + "rad"
    return os << B._degrees << "°, " << B._degrees * M_PI / 180 << "rad";
  }
};
}  // namespace lib2131
