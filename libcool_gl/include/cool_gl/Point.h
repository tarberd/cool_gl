#pragma once

#include <cool_gl/Colour.h>
#include <cool_gl/Drawable.h>

namespace cool_gl {

struct Point : public Drawable {
  Vec position;
  Colour colour;
  std::string m_name;

  Point() = delete;
  Point(const Point &) = default;
  Point(Point &&) = default;
  Point &operator=(const Point &) = default;
  Point &operator=(Point &&) = default;

  Drawable *copy() const noexcept;

  template <class A, class B, class C>
  Point(A &&position, B &&colour, C &&name) noexcept
      : position{std::forward<A>(position)}, colour{std::forward<B>(colour)},
        m_name{std::forward<C>(name)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec window_min,
            Vec window_max, Vec viewport_min, Vec viewport_max) const final;

  void transform(const Matrix &transform) noexcept;

  Vec mass_centre() noexcept;

  std::string type() const noexcept final;

  const std::string &name() const noexcept final;
  std::string &name() noexcept final;
};

} // namespace cool_gl
