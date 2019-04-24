#include "clipping.h"
#include <cool_gl/Line.h>
#include <iostream>

namespace cool_gl {

Drawable *Line::copy() const noexcept { return new Line{*this}; }

void Line::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
                Vec viewport_max) const {
  cr->set_line_width(width);

  Vec transformed_begin;
  Vec transformed_end;
  Vec clipped_begin;
  Vec clipped_end;


  auto window_min = Vec{-1.0, -1.0};
  auto window_max = Vec{1.0, 1.0};

  // TODO: Change this to a function that will choose between:
  // Cohen-Sutherland and a second algorithm
  std::vector<Vec> clipped_points = cohen_sutherland_clipping(begin, end, window_min, window_max);;

  double offset = 0.15;
  window_min = Vec{-1.0 - offset, -1.0 - offset};
  window_max = Vec{1.0 + offset, 1.0 + offset};

  if(!clipped_points.empty()){
    clipped_begin = clipped_points[0];
    clipped_end = clipped_points[1];

    transformed_begin.x = (clipped_begin.x - window_min.x) /
                          (window_max.x - window_min.x) *
                          (viewport_max.x - viewport_min.x);
    transformed_begin.y =
        (1 - (clipped_begin.y - window_min.y) / (window_max.y - window_min.y)) *
        (viewport_max.y - viewport_min.y);

    transformed_end.x = (clipped_end.x - window_min.x) / (window_max.x - window_min.x) *
                        (viewport_max.x - viewport_min.x);
    transformed_end.y =
        (1 - (clipped_end.y - window_min.y) / (window_max.y - window_min.y)) *
        (viewport_max.y - viewport_min.y);

    cr->set_source_rgb(colour.r, colour.g, colour.b);
    cr->move_to(transformed_begin.x, transformed_begin.y);
    cr->line_to(transformed_end.x, transformed_end.y);
    cr->stroke();
  }
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
