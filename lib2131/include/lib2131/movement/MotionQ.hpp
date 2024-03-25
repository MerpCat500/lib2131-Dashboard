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
bool isMotionQueued();

void clearMotions();
void exitMotion();
void queueMotion(motion_priorities_e priority);

}  // namespace lib2131
