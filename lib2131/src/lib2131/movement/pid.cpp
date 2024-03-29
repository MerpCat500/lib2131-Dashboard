#include "lib2131/movement/pid.hpp"

namespace lib2131
{
/**
 * PID Constructor
 * @brief Constructs a PID
 * @param PID_Controls Controls : Controls for a PID
 */
PID::PID(PID_Controls Controls)
    : _timer(timer()), _Controls(Controls), _last_I(0), _last_error(0)
{
}
/**
 * PID Constructor
 * @brief Constructs a PID
 * @param PID_Controls Controls : Controls for a PID
 * @param timer Timer : A timer to be used
 */
PID::PID(PID_Controls Controls, timer Timer)
    : _timer(Timer), _Controls(Controls), _last_I(0), _last_error(0)
{
}
/**
 * Update PID
 * @brief update a PID, calculate new output
 * @param  double error : Error = Target - Actual
 * @return double       :  PID * kPID
 */
double PID::update(double error)
{
  double Output(0);          // Init Output
  if (_Controls.isVelocity)  // Is Velocity Controller
  {
    P = error;  // P = Error
    // Trapezoidal Riemann Sum
    I += (error + _last_error) * _timer.get_time() / 2;
    // If Error's Sign Changed
    if (I * _last_I < 0)
    {
      I = 0;  // Remove I Component
    }
    // dError / dTime
    D = (error - _last_error) / _timer.get_time();

    // Update Last
    _last_error = error;
    _last_I = I;

    // Output PID * kPID
    Output = P * _Controls.kP + I * _Controls.kI + D * _Controls.kD;
  }
  else
  {
    P = error;  // P = Error
    // Trapezoidal Riemann Sum
    I += (error + _last_error) * _timer.get_time() / 2;
    // dError / dTime
    D = (error - _last_error) / _timer.get_time();

    // Update Last
    _last_error = error;
    _last_I = I;

    // Ouput PID * kPID
    Output = P * _Controls.kP + I * _Controls.kI + D * _Controls.kD;
  }

  // Calculate Settled
  // if In large Error zone and Over Large Settled Time
  if (fabs(Output) < _Controls.Exit_Conditions._large_error &&
      _timer.get_time() > _Controls.Exit_Conditions._large_time)
  {
    _settled = true;  // Is Settled
  }
  // If in Small Error zone and Over Small Settled Time
  else if (fabs(Output) < _Controls.Exit_Conditions._small_error &&
           _timer.get_time() > _Controls.Exit_Conditions._small_time)
  {
    _settled = true;  // Is Settled
  }
  else  // Otherwise
  {
    _settled = false;  // Isn't Settled
  }
}
/**
 * Get Settled
 * @brief Get if PID is settled
 * @return bool : is Settled?
 */
bool PID::get_settled() { return _settled; }
/**
 * Get Type
 * @brief Get if PID is a Velocity PID
 * @return bool : is Velocity?
 */
bool PID::get_Type() { return _Controls.isVelocity; }
/**
 * Reset PID
 * @brief Reset PID Values and Timer
 */
void PID::reset()
{
  P = 0;
  I = 0;
  D = 0;
  _timer.reset();
  _last_error = 0;
  _last_I = 0;
}
}  // namespace lib2131
