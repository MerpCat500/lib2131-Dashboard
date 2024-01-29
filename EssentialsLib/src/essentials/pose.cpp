#include "essentials/pose.hpp"

namespace essentials
{
  Pose::Pose(float x, float y, float theta, bool degrees = true):
    x(x),
    y(y),
    theta(theta),
    degrees(degrees)
  {
  }
        
  // Addition / Subtraction
  Pose Pose::operator+(Pose& pose2)
  {
    return {this->x + pose2.x, this->y + pose2.y, this->getDeg() + pose2.getDeg()};
  }

  Pose Pose::operator-(Pose& pose2)
  {
    return {this->x - pose2.x, this->y - pose2.y, this->getDeg() - pose2.getDeg()};
  }

  // Scale
  Pose Pose::operator*(float scalar)
  {
    return {this->x * scalar, this->y * scalar, this->getDeg() * scalar};
  }

  
  // Distance / Angle to Pose
  float Pose::distance(Pose pose2)
  {
    return sqrt(pow(pose2.x - this->x, 2) + pow(pose2.y - this->y, 2));
  }

  float Pose::angle(Pose pose2)
  {
    return atan2(pose2.x - this->x, pose2.y - this->y);
  }

  // Angles
  float Pose::getDeg()
  {
    return degrees ? theta : radToDeg(theta);
  }

  float Pose::getRad()
  {
    return degrees ? degToRad(theta) : theta;
  }
} // namespace essentials
