#include "lib2131/movement/MotionQ.hpp"

namespace lib2131
{

int _active_ID(0);        // Current Active ID
bool _motion_running(0);  // Is there a motion Running
/**
 * IS MOTION RUNNING
 * @brief Returns if a motion is running
 * @return bool : Is there a motion?
 */
bool isMotionRunning() { return _motion_running; }  // Is Motion Running?
/**
 * CLEAR MOTION
 * @brief Clears Current motions
 */
void clearMotions()  // Remove Motions
{
  _motion_running = false;  // Stop Moving
  _active_ID++;             // Go to Next ID
}

/**
 * QUEUEUEUEUE MOTION
 * @brief Starts a new motion
 * @param  motion_priorities_e priority : Importance of Motion
 * @return int : Active ID
 */
int queueMotion(motion_priorities_e priority)
{
  // If Motion is lazy and their is a motion in progress
  if (priority == motion_priorities_e::lazy && _motion_running)
  {
    return 0;  // Quit
  }
  // If motion is normal
  else if (priority == motion_priorities_e::normal && _motion_running)
  {
    while (_motion_running)  // Wait for Current Motion
    {
      pros::delay(10);  // Delay of 10 msec
    }
  }
  // If Motion is Important
  else if (priority == motion_priorities_e::important && _motion_running)
  {
    clearMotions();  // Remove any blocking Motions
  }
  else  // This Should never Execute
  {
    std::cout << "PROBLEM: Queue Motion" << std::endl;  // Yikes! Problem
  }

  // Start New Motion
  _motion_running = true;

  return _active_ID;
}

/**
 * CHECK ID
 * @brief Checks if ID is the same as the current Motion
 * @param ID : Check ID
 * @return bool : is ID equal to the active ID?
 */
bool checkID(int ID) { return (ID == _active_ID); }

}  // namespace lib2131
