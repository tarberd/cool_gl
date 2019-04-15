#include <cmath>
#include <cool_gl/Vec.h>
#include <stdexcept>

namespace cool_gl {

const double &Vec::operator[](int index) const {
  switch (index) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  case 3:
    return a;
  default:
    throw std::out_of_range("Vec::operator[] out off bounds");
  }
}

double &Vec::operator[](int index) {
  switch (index) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  case 3:
    return a;
  default:
    throw std::out_of_range("Vec::operator[] out off bounds");
  }
}

double angle(const Vec &left, const Vec &right) noexcept {
  auto magnitude_left = std::sqrt(std::pow(left.x, 2) + std::pow(left.y, 2));
  auto magnitude_right = std::sqrt(std::pow(right.x, 2) + std::pow(right.y, 2));

  auto dotproduct = left.x * right.x + left.y * right.y;

  return std::acos(dotproduct / (magnitude_left * magnitude_right));
}

} // namespace cool_gl
