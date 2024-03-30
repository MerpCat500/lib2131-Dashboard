#pragma once

#include "pros/rtos.hpp"

namespace lib2131
{
/**
 * TIMER CLASS
 * @brief "A class for all your timing needs :D"
 */
class timer
{
 private:
  int _start_time;  // Time of Init

 public:
  /**
   * TIMER
   * @brief Constructs a Timer
   */
  timer();
  /**
   * TIMER
   * @brief Constructs a Timer
   * @param int start_time : Set time of initialization
   */
  timer(int start_time);
  /**
   * Get Time
   * @brief Get Time Elapsed since Init or Reset
   * @return double : Time since Init / Reset
   */
  double get_time();
  /**
   * Get Start Time
   * @brief get start time since Init / Reset
   * @return double : Start Time
   */
  double get_start_time();

  /**
   * Reset
   * @brief Resets / Tares Timer
   */
  void reset();
};

int get_global_time();

extern pros::Task timer_thread;  // Thread for all timers

}  // namespace lib2131
