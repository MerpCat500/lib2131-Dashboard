#pragma once

#include <iostream>

#include "lib2131/chassis/angle.hpp"

namespace lib2131
{
class pose
{
 private:
  double _x, _y;
  angle _theta;

 public:
  pose();
  pose(double x, double y, angle theta);
  pose(double x, double y, double theta, bool degrees = true);

  double const get_x();
  double const get_y();
  angle get_theta();

  void set_x(double x);
  void set_y(double y);
  void set_theta(angle theta);

  pose addition(pose B);
  pose subtract(pose B);
  pose multiply(double B);
  pose additionEquals(pose B);
  pose subtractEquals(pose B);
  pose multiplyEquals(double B);

  pose operator+(pose B);
  pose operator-(pose B);
  pose operator*(double B);
  pose operator+=(pose B);
  pose operator-=(pose B);
  pose operator*=(double B);

  friend std::ostream& operator<<(std::ostream& os, pose const& B)
  {
    return os << "[" << B._x << ", " << B._y << ", " << B._theta << "]";
  }
};
}  // namespace lib2131
