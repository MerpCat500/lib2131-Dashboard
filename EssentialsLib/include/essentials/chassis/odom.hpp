#pragma once

#include "essentials/chassis/chassis.hpp"
#include "essentials/pose.hpp"

namespace essentials {

void setSensors();

Pose getPose(bool radians = false);

void setPose(Pose pose, bool radians = false);

void update();

void init();
} // namespace essentials
