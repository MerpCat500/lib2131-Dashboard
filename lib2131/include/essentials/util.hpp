#pragma once

#include <vector>

#include "essentials/math/vector2.hpp"
#include "math.h"
#include "pros/rtos.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace essentials
{
extern int time;

float radToDeg(float radians);
float degToRad(float degrees);
float signedAngle(float angle);
float angleError(float target, float actual);
float avg(std::vector<float> array);
vector2 rotatePoint(vector2 point, float angle, vector2 origin = {0, 0},
                    bool degrees = true);

class timer
{
 private:
  int startTime;

 public:
  timer();

  void start();
  void reset();

  int getTime();
};
}  // namespace essentials
