#include "MotionQ.hpp"

namespace lib2131
{

bool _active_motion(0);
bool _queued_motion(0);

bool isMotionRunning() { return _active_motion; }
bool isMotionQueued() { return _queued_motion; }

void clearMotions()
{
  _active_motion = 0;
  _queued_motion = 0;
}

void exitMotion() { _active_motion = false; }

void queueMotion(motion_priorities_e priority)
{
  if (priority == motion_priorities_e::lazy)
  {
    if (_active_motion)
    {
      return;
    }
    else
    {
      _active_motion = true;
    }
  }
  else if (priority == motion_priorities_e::normal)
  {
    if (_active_motion)
    {
      _queued_motion = true;
      while (_active_motion)
      {
        pros::delay(10);
      }
      _queued_motion = false;
    }

    _active_motion = true;
  }
  else if (priority == motion_priorities_e::important)
  {
    _active_motion = false;
    _queued_motion = false;
    _active_motion = true;
  }
  else
  {
    std::cout << "MOTION has problem Q-ing. (Check Priority)" << std::endl;
  }
}

}  // namespace lib2131
