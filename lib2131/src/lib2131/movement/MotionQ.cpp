#include "lib2131/movement/MotionQ.hpp"

namespace lib2131
{

int _active_ID(0);
bool _motion_running(0);

bool isMotionRunning() { return _motion_running; }

void clearMotions() { _motion_running = false; }

int queueMotion(motion_priorities_e priority)
{
  if (priority == motion_priorities_e::lazy && _motion_running)
  {
    return 0;
  }
  else if (priority == motion_priorities_e::normal)
  {
    while (_motion_running)
    {
      pros::delay(10);
    }
  }
  else
  {
    std::cout << "PROBLEM: Queue Motion" << std::endl;
  }

  _active_ID++;
  _motion_running = true;

  return _active_ID;
}

bool checkID(int ID) { return (ID == _active_ID); }

}  // namespace lib2131
