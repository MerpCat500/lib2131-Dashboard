#include "essentials/chassis/trackingWheel.hpp"

namespace essentials
{
TrackingWheel::TrackingWheel(pros::ADIEncoder* encoder, float diameter,
                             float distance)
    : encoder(encoder), diameter(diameter), distance(distance)
{
}

TrackingWheel::TrackingWheel(pros::Rotation* encoder, float diameter,
                             float distance)
    : rotation(encoder), diameter(diameter), distance(distance)
{
}

TrackingWheel::TrackingWheel(pros::Motor_Group* motors, float diameter,
                             float distance, float rpm)
    : motors(motors), diameter(diameter), distance(distance)
{
}

void TrackingWheel::reset()
{
  if (this->encoder != nullptr) this->encoder->reset();
  if (this->rotation != nullptr) this->rotation->reset_position();
  if (this->motors != nullptr) this->motors->tare_position();
}

float TrackingWheel::getDistanceTraveled()
{
  if (this->encoder != nullptr)
  {
    return (this->encoder->get_value() * this->diameter * M_PI / 360);
  }
  else if (this->rotation != nullptr)
  {
    return (this->rotation->get_position() / 100 * this->diameter * M_PI / 360);
  }
  else if (this->motors != nullptr)
  {
    // get distance traveled by each motor
    std::vector<pros::motor_gearset_e_t> gearsets = this->motors->get_gearing();
    std::vector<double> positions = this->motors->get_positions();
    std::vector<float> distances;
    for (int i = 0; i < this->motors->size(); i++)
    {
      float cart;
      switch (gearsets[i])
      {
        case pros::E_MOTOR_GEARSET_36:
          cart = 100;
          break;
        case pros::E_MOTOR_GEARSET_18:
          cart = 200;
          break;
        case pros::E_MOTOR_GEARSET_06:
          cart = 600;
          break;
        default:
          cart = 200;
          break;
      }
      distances.push_back(positions[i] * (diameter * M_PI) * (rpm / cart));
    }
    return essentials::avg(distances);
  }
  else
  {
    return 0;
  }
}

float TrackingWheel::getRaw()
{
  if (this->encoder != nullptr)
  {
    return this->encoder->get_value();
  }
  else if (this->rotation != nullptr)
  {
    return this->rotation->get_position();
  }
  else if (this->motors != nullptr)
  {
    return 0;
  }
}

float TrackingWheel::getOffset() { return this->distance; }
}  // namespace essentials
