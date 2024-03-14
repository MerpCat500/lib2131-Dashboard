#pragma once
#include "essentials/math/vector2.hpp"
#include "essentials/util.hpp"
namespace essentials
{

typedef class Pose
{
 private:
  bool degrees;
  float theta;

 public:
  vector2 pos;

  Pose(float x, float y, float theta, bool degrees = true);
  Pose(vector2 pos, float theta, bool degrees = true);

  // Addition / Subtraction
  Pose operator+(Pose B);
  Pose operator-(Pose B);

  // Scale
  Pose operator*(float scalar);

  // Angles
  void setTheta(float angle, bool degrees = true);
  float getDeg();
  float getRad();
};
}  // namespace essentials
