#pragma once

#include <iostream>

#include "pros/rtos.hpp"

namespace lib2131
{

enum class motion_priorities_e
{
  lazy,      // Cancel if motion
  normal,    // Wait for motion (queue)
  important  // Take motion (fast pass)
};

bool isMotionRunning();

void clearMotions();

int queueMotion(motion_priorities_e priority);
bool checkID(int ID);

}  // namespace lib2131
