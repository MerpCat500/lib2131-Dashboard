#include "essentials/util.hpp"

namespace essentials
{
int time(0);

float radToDeg(float radians) { return radians * 180 / M_PI; }

float degToRad(float degrees) { return degrees * M_PI / 180; }

float signedAngle(float angle) { return (angle > 180) ? angle - 360 : angle; }

float angleError(float target, float actual)
{
  return signedAngle(target) - signedAngle(actual);
}

float avg(std::vector<float> array)
{
  float total = 0;
  for (int i = 0; i < array.size(); i++)
  {
    total += array[i];
  }

  return total / (array.size() + 1);
}

vector2 rotatePoint(vector2 point, float angle, vector2 origin, bool degrees)
{
  if (degrees) angle = degToRad(angle);
  return {
      cos(angle) * (point.x - origin.x) + sin(angle) * (point.y - origin.y),
      sin(angle) * (point.x - origin.x) + cos(angle) * (point.y - origin.y)};
}

void timerHandler()
{
  while (1)
  {
    time += 10;
    pros::delay(10);
  }
}

timer::timer() : startTime(0) {}
void timer::start() { startTime = time; }
void timer::reset() { startTime = time; }
int timer::getTime() { time - startTime; }

}  // namespace essentials
