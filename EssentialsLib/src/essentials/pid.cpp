#include "essentials/pid.hpp"

namespace essentials
{

// Constructor
PID::PID(float kP, float kI, float kD, bool velocity)
{
  this->kP = kP;
  this->kI = kI;
  this->kD = kD;

  this->velocity = velocity;

  this->ErrorTimer = timer();
}

// Deconstruct
PID::~PID() {}

void PID::setGains(float kP, float kI, float kD)
{
  this->kP = kP;
  this->kI = kI;
  this->kD = kD;
}

void PID::setExit(PID_exitParam Exits) { this->exitConditions = Exits; }
void PID::reset()
{  // Error
  this->prevError = 0;
  this->totalError = 0;
  this->prevOutput = 0;
}

float PID::calculate(float target, float actual, float dT)
{
  if (velocity)
  {
    float error = (target - actual);
    float P = error;
    float I = (1 / 2) * (error + prevError) * dT;
    float D = (prevError - error) / dT;

    // If the last error (-) * error (+) is < 0 (aka passed 0)
    // "Take Back Half" if error passes target (limits oscillation)
    // if (prevError * error < 0)
    // {
    //   I *= 0.5;
    // }

    float Output = P * kP + I * kI + D * kD;

    if (fabs(error) < exitConditions.largeError)
    {
      ErrorTimer.start();
    }
    else
    {
      ErrorTimer.reset();
    }

    prevOutput = Output;
    return prevOutput + Output;
  }
  else
  {
    float error = (target - actual);
    float P = error;
    float I = (1 / 2) * (error + prevError) * dT;
    float D = (prevError - error) / dT;

    if (fabs(error) < exitConditions.largeError)
    {
      ErrorTimer.start();
    }
    else
    {
      ErrorTimer.reset();
    }

    float Output = P * kP + I * kI + D * kD;

    prevOutput = Output;
    return Output;
  }
}

bool PID::settled()
{
  if (fabs(prevError) < exitConditions.largeError)
  {
    if (ErrorTimer.getTime() > exitConditions.largeTime)
    {
      return 1;
    }
    else if (fabs(prevError) < exitConditions.smallError &&
             ErrorTimer.getTime() < exitConditions.smallTime)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

}  // namespace essentials
