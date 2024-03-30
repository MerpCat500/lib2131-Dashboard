#include "lib2131/movement/timer.hpp"

namespace lib2131
{

double time;  // Init Global Time Variable

/**
 * Timer
 * @brief Constructs a timer with start time = to global time
 * @return timer : Timer
 */
timer::timer() : _start_time(time) {}
/**
 * Timer
 * @brief Constructs a timer
 * @param int start_time : Sets Initial Time
 * @return timer : Timer
 */
timer::timer(int start_time) : _start_time(start_time) {}
/**
 * Get Time
 * @brief Returns time since Init / Reset
 * @return double : Time
 */
double timer::get_time() { return time - _start_time; }
/**
 * Get Start Time
 * @brief Returns Initialized Time
 * @return double : Start Time
 */
double timer::get_start_time() { return _start_time; }
/**
 * Reset
 * @brief Sets start time to current time
 */
void timer::reset() { _start_time = time; }

int get_global_time() { return time; }

/**
 * Under-laying Timer
 * @brief Is a G :D. "Works 24/7".
 */
void _timer()
{
  time = 0;  // Init Time
  while (1)  // Forever
  {
    time += 1;       // time +1 Msec
    pros::delay(1);  // Wait a msec
  }
}

pros::Task timer_thread(_timer, "TIMER PROCESS");
}  // namespace lib2131
