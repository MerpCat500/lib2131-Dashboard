#include "essentials/math/spline.hpp"

namespace essentials
{

hermiteSpline::~hermiteSpline() {}

Pose hermiteSpline::position(float t)
{
  return P0 * (2 * t * t * t - 3 * t * t + 1) +
         V0 * (t * t * t - 2 * t * t + t) + P1 * (3 * t * t - 2 * t * t * t) +
         V1 * (t * t * t - t * t);
}
Pose hermiteSpline::velocity(float t)
{
  return P0 * (6 * t * t - 6 * t) + V0 * (3 * t * t - 4 * t + 1) +
         P1 * (6 * t - 6 * t * t) + V1 * (3 * t * t - 2 * t);
}
Pose hermiteSpline::acceleration(float t)
{
  return P0 * (12 * t - 6) + V0 * (6 * t - 4) + P1 * (6 - 12 * t) +
         V1 * (6 * t - 2);
}

}  // namespace essentials
