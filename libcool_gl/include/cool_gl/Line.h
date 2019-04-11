#pragma once

#include <cool_gl/Colour.h>
#include <cool_gl/Drawable.h>

namespace cool_gl {

struct Line : public Drawable {
  Vec begin;
  Vec end;

  Colour colour;

  std::string m_name;

  double width;

  Line() = default;
  Line(const Line &) = default;
  Line(Line &&) = default;
  Line &operator=(const Line &) = default;
  Line &operator=(Line &&) = default;

  Drawable *copy() const noexcept;

  template <class A, class B, class C, class D>
  Line(A &&begin, B &&end, C &&colour, D &&name, double width = 1.0) noexcept
      : begin{std::forward<A>(begin)}, end{std::forward<B>(end)},
        colour{std::forward<C>(colour)}, m_name{std::forward<D>(name)},
        width{width} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
            Vec viewport_max) const final;

  void transform(const Matrix &transform) noexcept;

  Vec mass_centre() noexcept;

  std::string type() const noexcept final;

  const std::string &name() const noexcept final;
  std::string &name() noexcept final;
};

} // namespace cool_gl
