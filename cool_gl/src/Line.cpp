#include <cool_gl/Line.h>

namespace cool_gl {

Drawable *Line::copy() const noexcept { return new Line{*this}; }

void Line::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
                Vec viewport_max) const {
  cr->set_line_width(width);

  Vec transformed_begin;
  Vec transformed_end;

  double offset = 0.15;
  auto window_min = Vec{-1.0 - offset, -1.0 - offset};
  auto window_max = Vec{1.0 + offset, 1.0 + offset};

  transformed_begin.x = (begin.x - window_min.x) /
                        (window_max.x - window_min.x) *
                        (viewport_max.x - viewport_min.x);
  transformed_begin.y =
      (1 - (begin.y - window_min.y) / (window_max.y - window_min.y)) *
      (viewport_max.y - viewport_min.y);

  transformed_end.x = (end.x - window_min.x) / (window_max.x - window_min.x) *
                      (viewport_max.x - viewport_min.x);
  transformed_end.y =
      (1 - (end.y - window_min.y) / (window_max.y - window_min.y)) *
      (viewport_max.y - viewport_min.y);

  cr->set_source_rgb(colour.r, colour.g, colour.b);
  cr->move_to(transformed_begin.x, transformed_begin.y);
  cr->line_to(transformed_end.x, transformed_end.y);
  cr->stroke();
}

void Line::transform(const Matrix &transform) noexcept {
  begin = multiply(transform, begin);
  end = multiply(transform, end);
}

Vec Line::mass_centre() noexcept {
  double x_centre = (begin.x + end.x) / 2.0;
  double y_centre = (begin.y + end.y) / 2.0;

  return Vec{x_centre, y_centre};
}

std::string Line::type() const noexcept { return "Line"; }

const std::string &Line::name() const noexcept { return m_name; }

std::string &Line::name() noexcept { return m_name; }

} // namespace cool_gl
