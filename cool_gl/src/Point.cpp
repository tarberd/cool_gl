#include <cool_gl/Point.h>

namespace cool_gl {

Drawable *Point::copy() const noexcept { return new Point{*this}; }

bool should_draw_point(Vec point) {
  if (std::abs(point.x) > 1 || std::abs(point.y) > 1) {
    return false;
  }
  return true;
}

void Point::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
                 Vec viewport_max) const {
  if (should_draw_point(position) == true) {
    Vec transformed_point;

    double offset = 0.15;
    auto window_min = Vec{-1.0 - offset, -1.0 - offset};
    auto window_max = Vec{1.0 + offset, 1.0 + offset};

    transformed_point.x = (position.x - window_min.x) /
                          (window_max.x - window_min.x) *
                          (viewport_max.x - viewport_min.x);
    transformed_point.y =
        (1 - (position.y - window_min.y) / (window_max.y - window_min.y)) *
        (viewport_max.y - viewport_min.y);

    cr->set_source_rgb(colour.r, colour.g, colour.b);
    cr->move_to(transformed_point.x, transformed_point.y);
    cr->arc(transformed_point.x, transformed_point.y, 3.0, 0.0, 2 * M_PI);
    cr->fill();
  }
}

void Point::transform(const Matrix &transform) noexcept {
  position = multiply(transform, position);
}

Vec Point::mass_centre() noexcept { return position; }

std::string Point::type() const noexcept { return "Point"; }

const std::string &Point::name() const noexcept { return m_name; }

std::string &Point::name() noexcept { return m_name; }

} // namespace cool_gl
