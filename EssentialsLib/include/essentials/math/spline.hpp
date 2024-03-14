#pragma once

#include "essentials/math/pose.hpp"

namespace essentials
{

class hermiteSpline
{
 private:
  Pose P0;
  Pose V0;
  Pose P1;
  Pose V1;

 public:
  hermiteSpline(Pose P0, Pose V0, Pose P1, Pose V1);

  ~hermiteSpline();

  Pose position(float t);
  Pose velocity(float t);
  Pose acceleration(float t);
};
}  // namespace essentials
