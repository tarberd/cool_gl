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

} // namespace cool_gl
