#include "essentials/math/pose.hpp"

namespace essentials
{
Pose::Pose(float x, float y, float theta, bool degrees)
{
  this->pos = {x, y};
  this->theta = theta;
  this->degrees = degrees;
}

Pose::Pose(vector2 pos, float theta, bool degrees)
{
  this->pos = pos;
  this->theta = theta;
  this->degrees = degrees;
}

// Angles
void Pose::setTheta(float angle, bool degrees)
{
  this->theta = angle;
  this->degrees = degrees;
}

Pose Pose::operator+(Pose B)
{
  return {this->pos.x + B.pos.x, this->pos.y + B.pos.y,
          this->getRad() + B.getRad()};
}

Pose Pose::operator-(Pose B)
{
  return {this->pos.x - B.pos.x, this->pos.y - B.pos.y,
          this->getRad() - B.getRad()};
}

Pose Pose::operator*(float scalar)
{
  return {this->pos.x * scalar, this->pos.y * scalar, this->getRad() * scalar};
}

float Pose::getDeg()
{
  return this->degrees ? this->theta : radToDeg(this->theta);
}
float Pose::getRad()
{
  return this->degrees ? degToRad(this->theta) : this->theta;
}

}  // namespace essentials
