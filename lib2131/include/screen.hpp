#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "pros/apix.h"
#include "pros/llemu.hpp"

namespace screen
{
struct Auton
{
  std::string name;
  std::string desc;
  std::string setUp;
  void (*callBack)(void);  // Callback

  /**
   * Empty Construction
   */
  Auton() {}

  /**
   *  Construction of an Auton
   */
  Auton(std::string Name, std::string Desc, std::string SetUp, void (*CB)(void))
      : name(Name), desc(Desc), setUp(SetUp), callBack(CB)
  {
  }
  ~Auton() {}
};

// Auton Array
extern std::vector<Auton> AutonList;

extern bool debug;
extern int index;

}  // namespace screen
