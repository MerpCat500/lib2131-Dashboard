#include "lib2131/movement/timer.hpp"

namespace lib2131
{

double time(0);

timer::timer() : _start_time(time) {}
timer::timer(int start_time) : _start_time(start_time) {}

double timer::get_time() { return time - _start_time; }
double timer::get_start_time() { return _start_time; }

void timer::reset() { _start_time = time; }

void _timer()
{
  time = 0;
  while (1)
  {
    time += 1;
    pros::delay(1);
  }
}

pros::Task timer_thread(_timer, "TIMER PROCESS");
}  // namespace lib2131
