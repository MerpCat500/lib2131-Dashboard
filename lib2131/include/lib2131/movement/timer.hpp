#pragma once

#include "pros/rtos.hpp"

namespace lib2131
{

class timer
{
 private:
  int _start_time;

 public:
  timer();
  timer(int start_time);

  double get_time();
  double get_start_time();

  void reset();
};

extern pros::Task timer_thread;

}  // namespace lib2131
