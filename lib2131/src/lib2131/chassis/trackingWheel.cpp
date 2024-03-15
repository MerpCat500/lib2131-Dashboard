#include "lib2131/chassis/trackingWheel.hpp"

namespace lib2131
{
trackingWheel::trackingWheel(pros::ADIEncoder* Encoder, double WheelSize,
                             double Offset, double Ratio = 1)
    : _Encoder(Encoder), _WheelSize(WheelSize), _Offset(Offset), _Ratio(Ratio)
{
}

trackingWheel::trackingWheel(pros::Rotation* Rotational, double WheelSize,
                             double Offset, double Ratio = 1)
    : _Rotational(Rotational),
      _WheelSize(WheelSize),
      _Offset(Offset),
      _Ratio(Ratio)
{
}

trackingWheel::trackingWheel(pros::Motor_Group* Motor_Group, double WheelSize,
                             double Offset, double Ratio)
    : _Motor_Group(Motor_Group),
      _WheelSize(WheelSize),
      _Offset(Offset / 2),
      _Ratio(Ratio)
{
}

void trackingWheel::set_Dist(double Distance) { _Dist_Traveled = Distance; }
double trackingWheel::get_Raw()
{
  if (_Encoder != nullptr)
  {
    return _Encoder->get_value();
  }
  else if (_Rotational != nullptr)
  {
    return _Rotational->get_position() / 100;
  }
  else if (_Motor_Group != nullptr)
  {
    // get distance traveled by each motor
    std::vector<pros::motor_gearset_e_t> gearsets = _Motor_Group->get_gearing();
    std::vector<double> positions = _Motor_Group->get_positions();
    double total = 0;
    for (int i = 0; i < _Motor_Group->size(); i++)
    {
      int Cart_Type;
      switch (gearsets[i])
      {
        case pros::E_MOTOR_GEARSET_36:
          Cart_Type = 100;
          break;
        case pros::E_MOTOR_GEARSET_18:
          Cart_Type = 200;
          break;
        case pros::E_MOTOR_GEARSET_06:
          Cart_Type = 600;
          break;
        default:
          Cart_Type = 200;
          break;
      }

      total += (positions[i] * (_Ratio / Cart_Type));
    }
    return total / _Motor_Group->size();
  }
  else
  {
    std::cout << "PROBLEM W/ Tracking wheels" << std::endl;
    return 0;
  }
}
double trackingWheel::get_Dist() { return _Dist_Traveled; }
double trackingWheel::get_d_Dist() { return _d_Dist_Traveled; }
double trackingWheel::get_Offset() { return _Offset; }

void trackingWheel::update()
{
  // Pick Sensor
  if (_Encoder != nullptr)
  {
    // Calc New Dist Traveled
    _Dist_Traveled = (_Encoder->get_value() / 360 * _WheelSize * M_PI) / _Ratio;
    // Calc Change
    _d_Dist_Traveled = _Dist_Traveled - _last_Dist_Traveled;
    // Update Last Dist
    _last_Dist_Traveled = _Dist_Traveled;
  }
  else if (_Rotational != nullptr)
  {
    // Calc New Dist
    _Dist_Traveled =
        (_Rotational->get_position() / 36000 * _WheelSize * M_PI) / _Ratio;
    // Calc Change
    _d_Dist_Traveled = _Dist_Traveled - _last_Dist_Traveled;
    // Update Last Dist
    _last_Dist_Traveled = _Dist_Traveled;
  }
  else if (_Motor_Group != nullptr)
  {
    // Calc Dist Traveled
    _Dist_Traveled = (this->get_Raw() / 360 * _WheelSize * M_PI);
    // Calc Change
    _d_Dist_Traveled = _Dist_Traveled - _last_Dist_Traveled;
    // Update Last Dist
    _last_Dist_Traveled = _Dist_Traveled;
  }
}
void trackingWheel::reset()
{
  if (_Encoder != nullptr)
  {
    _Encoder->reset();
  }
  else if (_Rotational != nullptr)
  {
    _Rotational->reset_position();
  }
  else if (_Motor_Group != nullptr)
  {
    _Motor_Group->tare_position();
    _Motor_Group->set_encoder_units(
        pros::motor_encoder_units_e::E_MOTOR_ENCODER_DEGREES);
  }

  _Dist_Traveled = 0;
  _d_Dist_Traveled = 0;
}
}  // namespace lib2131
