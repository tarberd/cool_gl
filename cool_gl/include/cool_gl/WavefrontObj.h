#pragma once

#include "Drawable.h"
#include "Polygon.h"
#include <istream>

namespace cool_gl {

class WavefrontObj : public Drawable {
public:
  std::vector<Vec> vertex_data;

  std::vector<std::tuple<int, int, int>> faces;

  Colour colour;

  std::string m_name;

  double width;

  Drawable *copy() const noexcept;

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
            Vec viewport_max) const final;

  void transform(const Matrix &transform) noexcept;

  WavefrontObj() = delete;
  WavefrontObj(const WavefrontObj &) = default;
  WavefrontObj(WavefrontObj &&) = default;
  WavefrontObj &operator=(const WavefrontObj &) = default;
  WavefrontObj &operator=(WavefrontObj &&) = default;

  template <class A, class B, class C, class D>
  WavefrontObj(A &&vertex_data, B &&faces, C &&colour, D &&name,
               double width = 1.0) noexcept
      : vertex_data{std::forward<A>(vertex_data)},
        faces{std::forward<B>(faces)}, colour{std::forward<C>(colour)},
        m_name{std::forward<D>(name)}, width{width} {}

  Vec mass_centre() noexcept;

  std::string type() const noexcept;

  const std::string &name() const noexcept;
  std::string &name() noexcept;
};

WavefrontObj make_polygon_from_obj(std::istream &input) noexcept;
}
