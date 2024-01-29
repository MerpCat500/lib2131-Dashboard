#pragma once

#include <vector>
#include "math.h"

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

namespace essentials 
{

float radToDeg(float radians);
float degToRad(float degrees);
float signedAngle(float angle);
float angleError(float target, float actual);

} // namespace essentials
