#pragma once
#include <cmath>
#include <iostream>

namespace lib2131
{
class angle
{
 private:
  double _radians, _degrees;

 public:
  angle(double value, bool isDegrees = true);

  double const get_deg();
  double const get_rad();

  angle addition(angle B);
  angle subtract(angle B);
  angle multiply(double B);

  angle additionEqual(angle B);
  angle subtractEqual(angle B);
  angle multiplyEqual(double B);

  angle operator+(angle B);
  angle operator-(angle B);
  angle operator*(double B);

  angle operator+=(angle B);
  angle operator-=(angle B);
  angle operator*=(double B);

  friend std::ostream& operator<<(std::ostream& os, angle const& B)
  {
    return os << B._degrees << "°, " << B._radians << "rad";
  }
};
}  // namespace lib2131
