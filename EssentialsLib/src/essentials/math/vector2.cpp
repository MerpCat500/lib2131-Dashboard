#include "essentials/math/vector2.hpp"

namespace essentials
{
vector2::vector2() {}
vector2::vector2(float x, float y) : x(x), y(y) {}

vector2::~vector2() {}
// Distance / Angle to Pose
float vector2::distance(vector2& B)
{
  return sqrt(pow(B.x - this->x, 2) + pow(B.y - this->y, 2));
}

float vector2::angle(vector2& B) { return atan2(B.x - this->x, B.y - this->y); }

// Addition / Subtraction
vector2 vector2::operator+(vector2& B)
{
  return {this->x + B.x, this->y + B.y};
}

void vector2::operator+=(vector2 B)
{
  this->x += B.x;
  this->y += B.y;
}

vector2 vector2::operator-(vector2& B)
{
  return {this->x - B.x, this->y - B.y};
}

// Scale
vector2 vector2::operator*(float scalar)
{
  return {this->x * scalar, this->y * scalar};
}

vector2 vector2::polar()
{
  vector2 origin = {0, 0};
  return {this->distance(origin), this->angle(origin)};
}

std::ostream& operator<<(std::ostream& os, const vector2& Vector)
{
  os << Vector.x << ", " << Vector.y;
  return os;
}

}  // namespace essentials
