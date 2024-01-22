#pragma once

namespace essentials {
class Pose {
    public:
        float x;
        float y;
        float theta;

        Pose(float x, float y, float theta);
        
        // Addition / Subtraction
        Pose operator+(const Pose& pose2);
        Pose operator-(const Pose& pose2);

        // Scale
        Pose operator*(const float scalar);
        Pose operator/(const float& other);
        
        // Distance / Angle to Pose
        float distance(Pose other);
        float angle(Pose other);
};
} // namespace essentials
