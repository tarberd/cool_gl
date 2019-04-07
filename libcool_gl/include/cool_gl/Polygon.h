#pragma once

#include <cool_gl/Colour.h>
#include <cool_gl/Drawable.h>

namespace cool_gl {

struct Polygon : public Drawable {
  std::vector<Vec> points;

  Colour colour;

  std::string m_name;

  Polygon() = delete;
  Polygon(const Polygon &) = default;
  Polygon(Polygon &&) = default;
  Polygon &operator=(const Polygon &) = default;
  Polygon &operator=(Polygon &&) = default;

  Drawable *copy() const noexcept;

  template <class A, class B, class C>
  Polygon(A &&points, B &&colour, C &&name) noexcept
      : points{std::forward<A>(points)}, colour{std::forward<B>(colour)},
        m_name{std::forward<C>(name)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Window window,
            Vec viewport_min, Vec viewport_max) const final;

  void transform(const Matrix &transform) noexcept;

  Vec mass_centre() noexcept;

  std::string type() const noexcept final;

  const std::string &name() const noexcept final;
  std::string &name() noexcept final;
};

} // namespace cool_gl

