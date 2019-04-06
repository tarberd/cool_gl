#pragma once

namespace cool_gl {

struct Colour {
  double r{0.0};
  double g{0.0};
  double b{0.0};

  Colour() = default;
  Colour(const Colour &) = default;
  Colour(Colour &&) = default;
  Colour(double r, double g, double b) : r{r}, g{g}, b{b} {}
};

} // namespace cool_gl
