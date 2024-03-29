#pragma once

#include <iostream>

#include "pros/rtos.hpp"

namespace lib2131
{
/**
 * MOTION PRIORITIES
 * @brief "Get your priorities straight"
 */
enum class motion_priorities_e
{
  /**
   * motion_priorities_e
   * @brief How should a motion execute?
   * @param lazy : Cancel if other motion
   * @param normal : Wait if other motion
   * @param lazy : Kill if other motion
   */
  lazy,
  normal,
  important
};

/**
 * IS MOTION RUNNING?
 * @brief Returns is a Motion is running
 * @return bool  : is a Motion Running
 */
bool isMotionRunning();

/**
 * Clear Motions
 * @brief Removes Current Motion and moves to next active ID
 */
void clearMotions();

/**
 * QUEUE MOTION
 * @brief Start a new motion
 * @param  motion_priorities_e priority : importance of motion
 * @return int : A Running ID for motion
 */
int queueMotion(motion_priorities_e priority);
/**
 * Check ID
 * @param int ID : ID to be checked
 * @return bool : Is Checked ID still active?
 */
bool checkID(int ID);

}  // namespace lib2131
