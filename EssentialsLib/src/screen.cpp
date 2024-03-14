#include "screen.hpp"

#include "essentials/api.hpp"

namespace screen
{
bool debug(1);

void screenHandler()
{
  pros::lcd::initialize();
  int i = 0;
  while (true)  // Forever
  {
    if (debug)  // If I want the position on screen for tuning
    {
      pros::lcd::clear();
      essentials::Pose position =
          essentials::odom::getPose();  // Display robot's current position
      pros::lcd::print(1, "Position:");
      pros::lcd::print(2, "(%f, %f, %f)", position.pos.x, position.pos.y,
                       position.getDeg());
    }
    pros::delay(50);  // Make sure the thread doesn't execute too quickly
  }
}

// Run the screenHandler in it's own thread
pros::Task screenThread(screenHandler);
}  // namespace screen