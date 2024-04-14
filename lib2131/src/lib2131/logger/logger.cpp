#include "lib2131/logger/logger.hpp"

namespace lib2131
{
namespace logger
{

std::string Command = "Start";

void logger()
{
  while (1)
  {
    std::cout << odom::get_pose() << std::endl;
    std::cin >> Command;
  }
}

void init() { pros::Task loggerThread(logger, "Logging Thread"); }
}  // namespace logger

}  // namespace lib2131
