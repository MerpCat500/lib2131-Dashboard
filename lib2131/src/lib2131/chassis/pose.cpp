#include "lib2131/chassis/pose.hpp"

namespace lib2131
{
pose::pose() : _theta(0, 0) {}
pose::pose(double x, double y, angle theta) : _theta(theta), _x(x), _y(y) {}
pose::pose(double x, double y, double theta, bool isDegrees)
    : _theta({theta, isDegrees}), _x(x), _y(y)
{
}

double const pose::get_x() { return _x; }
double const pose::get_y() { return _y; }
angle pose::get_theta() { return _theta; }

void pose::set_x(double x) { _x = x; }
void pose::set_y(double y) { _y = y; }
void pose::set_theta(angle theta) { _theta = theta; }

pose pose::addition(pose B)
{
  return {this->_x + B.get_x(), this->_y + B.get_y(),
          this->get_theta() + B.get_theta()};
}
pose pose::subtract(pose B)
{
  return {this->_x - B.get_x(), this->_y - B.get_y(),
          this->get_theta() - B.get_theta()};
}
pose pose::multiply(double B)
{
  return {this->_x * B, this->_y * B, this->get_theta() * B};
}

pose pose::additionEquals(pose B)
{
  this->_x += B.get_x();
  this->_y += B.get_y();
  this->_theta += B.get_theta();
  return (*this);
}
pose pose::subtractEquals(pose B)
{
  this->_x -= B.get_x();
  this->_y -= B.get_y();
  this->_theta -= B.get_theta();
  return (*this);
}
pose pose::multiplyEquals(double B)
{
  this->_x *= B;
  this->_y *= B;
  this->_theta *= B;
  return (*this);
}

pose pose::operator+(pose B) { return addition(B); }
pose pose::operator-(pose B) { return subtract(B); }
pose pose::operator*(double B) { return multiply(B); }

pose pose::operator+=(pose B) { return additionEquals(B); }
pose pose::operator-=(pose B) { return subtractEquals(B); }
pose pose::operator*=(double B) { return multiplyEquals(B); }

}  // namespace lib2131
