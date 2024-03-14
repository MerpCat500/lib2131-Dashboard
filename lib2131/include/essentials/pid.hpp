#pragma once
#include <string>

#include "essentials/util.hpp"

namespace essentials
{

struct PID_exitParam
{
  float largeError;
  float smallError;
  int largeTime;
  int smallTime;
};

class PID
{
 private:
  // Tuning Values
  float kP;
  float kI;
  float kD;

  // Type
  bool velocity;

  // Exit Conditions
  timer ErrorTimer;
  PID_exitParam exitConditions;
  bool settled;

  // Error
  float prevError;
  float prevOutput;

 public:
  PID(float kP, float kI, float kD, bool velocity);
  ~PID();

  void setGains(float kP, float kI, float kD);
  void setExit(PID_exitParam Exits);

  float calculate(float target, float actual, float dT);
  void reset();
  bool settled();
};
}  // namespace essentials
