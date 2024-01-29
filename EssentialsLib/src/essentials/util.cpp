#include "essentials/util.hpp"

namespace essentials
{
    
float radToDeg(float radians)
{
  return radians * 180 / M_PI;
}

float degToRad(float degrees)
{
  return degrees * M_PI / 180;
}

float signedAngle(float angle)
{
  return (angle > 180) ? angle - 360 : angle;
}

float angleError(float target, float actual)
{
  return signedAngle(target) - signedAngle(actual); 
}

} // namespace essentials
