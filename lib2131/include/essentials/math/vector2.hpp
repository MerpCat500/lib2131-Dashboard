#pragma once
#include <cmath>
#include <iostream>

namespace essentials
{
struct vector2
{
  float x;
  float y;

  vector2();
  vector2(float x, float y);

  ~vector2();

  // Addition / Subtraction
  void operator+=(vector2 B);
  vector2 operator+(vector2& B);
  vector2 operator-(vector2& B);

  // Scale
  vector2 operator*(float scalar);

  float distance(vector2& B);
  float angle(vector2& B);

  vector2 polar();
};

std::ostream& operator<<(std::ostream&, const vector2&);

}  // namespace essentials
