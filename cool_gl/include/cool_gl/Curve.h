#pragma once

#include <cool_gl/Colour.h>
#include <cool_gl/Drawable.h>

namespace cool_gl {
  struct Curve : public Drawable {
    Vec begin;
    Vec end;

    std::vector<Vec> control_points;

    Colour colour;

    std::string m_name;

    double width;

    Curve() = delete;
    Curve(const Curve &) = default;
    Curve(Curve &&) = default;
    Curve &operator=(const Curve &) = default;
    Curve &operator=(Curve &&) = default;

    Drawable *copy() const noexcept;

    template <class A, class B, class C, class D, class E>
    Curve(A &&begin, B &&end, C &&control_points, D &&colour, E &&name,
          double width = 1.0) noexcept
        : begin{std::forward<A>(begin)}, end{std::forward<B>(end)},
          control_points{std::forward<C>(control_points)},
          colour{std::forward<D>(colour)}, m_name{std::forward<E>(name)},
          width{width} {}

    void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
              Vec viewport_max) const final;

    std::string type() const noexcept final;

    void transform(const Matrix &transform) noexcept;

    Vec mass_centre() noexcept;

    const std::string &name() const noexcept final;
    std::string &name() noexcept final;

    std::vector<Vec> bezier_curve() const noexcept;
  };
} // namespace cool_gl
