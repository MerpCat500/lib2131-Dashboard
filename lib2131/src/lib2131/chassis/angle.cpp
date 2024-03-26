#include "lib2131/chassis/angle.hpp" 
namespace lib2131
{
angle::angle(double value, bool isDegrees)
{
  if (isDegrees)
  {
    this->_degrees = value;
    this->_radians = value * M_PI / 180;
  }
  else
  {
    this->_radians = value;
    this->_degrees = value * 180 / M_PI;
  }
}

angle angle::addition(angle B)
{
  return angle(this->_degrees + B.get_deg(), true);
}
angle angle::subtract(angle B)
{
  return angle(this->_degrees - B.get_deg(), true);
}
angle angle::multiply(double B) { return angle(this->_degrees * B, true); }

angle angle::additionEqual(angle B)
{
  this->_degrees += B.get_deg();
  this->_radians += B.get_rad();
  return (*this);
}

angle angle::subtractEqual(angle B)
{
  this->_degrees -= B.get_deg();
  this->_radians -= B.get_rad();
  return (*this);
}
angle angle::multiplyEqual(double B)
{
  this->_degrees *= B;
  this->_radians *= B;
  return (*this);
}

angle angle::wrapped() { return angle(std::fmod(this->_degrees, 360), true); }

angle angle::operator+(angle B) { return addition(B); }
angle angle::operator-(angle B) { return subtract(B); }
angle angle::operator*(double B) { return multiply(B); }

angle angle::operator+=(angle B) { return additionEqual(B); }
angle angle::operator-=(angle B) { return subtractEqual(B); }
angle angle::operator*=(double B) { return multiplyEqual(B); }

double const angle::get_deg() { return _degrees; }
double const angle::get_rad() { return _radians; }
}  // namespace lib2131
