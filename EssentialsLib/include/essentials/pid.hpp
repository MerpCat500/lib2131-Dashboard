#pragma once
#include <string>
#include "pros/rtos.hpp"

namespace essentials {

class PID {
    private:
        // Tuning Values
        float kF;
        float kP;
        float kI;
        float kD;

        // Exit Conditions
        float largeError;
        float smallError;
        int largeTime = 0;
        int smallTime = 0;
        int maxTime = -1; // -1 means no max time set, run forever

        int largeTimeCounter = 0;
        int smallTimeCounter = 0;
        int startTime = 0;

        // Error
        float prevError = 0;
        float totalError = 0;
        float prevOutput = 0;
        
    public:       
        PID(float kF, float kP, float kI, float kD);

        void setGains(float kF, float kP, float kI, float kD);
        void setExit(float largeError, float smallError, int largeTime, int smallTime, int maxTime);
        
        float update(float target, float actual);
        void reset();
        bool settled();

};
} // namespace essentials
