#include "lib2131/movement/pid.hpp"

namespace lib2131
{
PID::PID(PID_Controls Controls)
    : _timer(timer()), _Controls(Controls), _last_I(0), _last_error(0)
{
}
PID::PID(PID_Controls Controls, timer Timer)
    : _timer(Timer), _Controls(Controls), _last_I(0), _last_error(0)
{
}

double PID::update(double error)
{
  double Output(0);
  if (_Controls.isVelocity)
  {
    P = error;
    I += (error + _last_error) * _timer.get_time() / 2;
    if (I * _last_I < 0)
    {
      I = 0;
    }
    D = (error - _last_error) / _timer.get_time();

    _last_error = error;
    _last_I = I;
    Output = P * _Controls.kP + I * _Controls.kI + D * _Controls.kD;
  }
  else
  {
    P = error;
    I += (error + _last_error) * _timer.get_time() / 2;
    D = (error - _last_error) / _timer.get_time();

    _last_error = error;
    _last_I = I;
    Output = P * _Controls.kP + I * _Controls.kI + D * _Controls.kD;
  }

  if (fabs(Output) < _Controls.Exit_Conditions._large_error &&
      _timer.get_time() > _Controls.Exit_Conditions._large_time)
  {
    _settled = true;
  }
  else if (fabs(Output) < _Controls.Exit_Conditions._small_error &&
           _timer.get_time() > _Controls.Exit_Conditions._small_time)
  {
    _settled = true;
  }
  else
  {
    _settled = false;
  }
}

bool PID::get_settled() { return _settled; }

bool PID::get_Type() { return _Controls.isVelocity; }

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
