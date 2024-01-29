#pragma once
#include "essentials/util.hpp"

namespace essentials {
class Pose {
    public:
        float x;
        float y;

        float theta;
        bool degrees;

        Pose(float x, float y, float theta, bool degrees = true);
        
        // Addition / Subtraction
        Pose operator+(Pose& pose2);
        Pose operator-(Pose& pose2);

        // Scale
        Pose operator*(float scalar);
        
        // Distance / Angle to Pose
        float distance(Pose other);
        float angle(Pose other);

        // Angles
        float getDeg();
        float getRad();
};
} // namespace essentials
