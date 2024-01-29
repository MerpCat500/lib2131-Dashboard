#pragma once

#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include "essentials/chassis/trackingWheel.hpp"
#include "essentials/pose.hpp"

namespace essentials {


class Chassis {
    public:
        
        Chassis();
        
        void calibrate();
        void setPose();
        
        Pose getPose();
        void turnTo();
        void moveTo();
        
};
} // namespace essentials
