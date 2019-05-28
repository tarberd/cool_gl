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

    Curve() = default;
    Curve(const Curve &) = default;
    Curve(Curve &&) = default;
    Curve &operator=(const Curve &) = default;
    Curve &operator=(Curve &&) = default;

    Drawable *copy() const noexcept;

    template <class A, class B, class C, class D>
    Curve(A &&control_points, B &&colour, C &&name, double width = 1.0) noexcept
        : control_x{std::forward<A>(control_points)},
          colour{std::forward<B>(colour)},
          m_name{std::forward<C>(name)}, width{width} {}

    void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
              Vec viewport_max) const final;

    std::vector<cool_gl::Line> bezier_curve();
    std::string type() const noexcept final;

    void transform(const Matrix &transform) noexcept;

    Vec mass_centre() noexcept;


    const std::string &name() const noexcept final;
    std::string &name() noexcept final;

  };
} // namespace cool_gl
