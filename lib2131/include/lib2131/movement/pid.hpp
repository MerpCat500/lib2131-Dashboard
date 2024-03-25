#pragma once

#include <cmath>

#include "lib2131/movement/timer.hpp"

namespace lib2131
{

struct PID_Controls
{
  double kP;
  double kI;
  double kD;

  PID_Exit Exit_Conditions;

  bool isVelocity;
};

struct PID_Exit
{
  double _small_error;
  double _small_time;

  double _large_error;
  double _large_time;
};

class PID
{
 private:
  PID_Controls _Controls;
  timer _timer;
  double _last_error;
  double _last_I;

  bool _settled;

  double P, I, D;

 public:
  PID(PID_Controls Controls);
  PID(PID_Controls Controls, timer Timer);

  double update(double error);
  bool get_settled();
  bool get_Type();

  void reset();
};

}  // namespace lib2131
