#pragma once

#include <ostream>

namespace cool_gl {

struct Vec {
  double x;
  double y;
  double z;
  double a;

  Vec() = default;
  Vec(const Vec &) = default;
  Vec &operator=(const Vec &) = default;
  Vec &operator=(Vec &&) = default;
  Vec(Vec &&) = default;

  Vec(double x, double y, double z = 1.0, double a = 1.0)
      : x{x}, y{y}, z{z}, a{a} {}

  const double &operator[](int x) const;
  double &operator[](int x);

  friend std::ostream &operator<<(std::ostream &os, const Vec &p);
};

std::ostream &operator<<(std::ostream &os, const Vec &p);

double angle(const Vec &left, const Vec &right) noexcept;

} // namespace cool_gl
