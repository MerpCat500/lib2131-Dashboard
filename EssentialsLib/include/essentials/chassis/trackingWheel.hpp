#pragma once

#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include "pros/rotation.hpp"

namespace essentials {
class TrackingWheel {
    private:
        float diameter;
        float distance;
        float rpm;
        
        float gearRatio = 1;

    public:
        TrackingWheel(pros::ADIEncoder* encoder, float diameter, float distance, float gearRatio = 1);
        TrackingWheel(pros::Rotation* encoder, float diameter, float distance, float gearRatio = 1);
        TrackingWheel(pros::Motor_Group* motors, float diameter, float distance, float rpm);
        
        void reset();
        float getDistanceTraveled();
        float getOffset();
          
};
} // namespace essentials