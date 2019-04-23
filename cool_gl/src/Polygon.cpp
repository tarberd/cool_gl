#include "clipping.h"
#include <cool_gl/Polygon.h>
#include <iostream>

namespace cool_gl {

Drawable *Polygon::copy() const noexcept { return new Polygon{*this}; }

std::vector<std::vector<Vec>> polygon_clipping(const std::vector<Vec> &in,
                                               const Vec &window_min,
                                               const Vec &window_max) {
  std::vector<Vec> enter_vertex;

  std::vector<std::vector<Vec>> clipped_polygon;

  enum class polygon_state { open, close };

  auto state = polygon_state::close;

  // Everything else
  for (int i = 0; i < in.size(); i++) {

    auto next = (i + 1) % in.size();

    // Check from out to in
    if (std::abs(in[i].x) > std::abs(window_min.x) &&
        std::abs(in[next].x) <= std::abs(window_min.x)) {
      if (state == polygon_state::close) {
        state = polygon_state::open;

        clipped_polygon.emplace_back();
      }

      auto &back = clipped_polygon.back();

      auto begin = in[i];

      begin.x = window_min.x;

      back.emplace_back(begin);
    }
    // Check from in to out
    else if (std::abs(in[i].x) <= std::abs(window_min.x) &&
             std::abs(in[next].x) > std::abs(window_min.x)) {
      if (state == polygon_state::open) {
        state = polygon_state::close;
      }
      if (state == polygon_state::close) {
        state = polygon_state::open;

        clipped_polygon.emplace_back();
      }

      auto &back = clipped_polygon.back();

      auto end = in[next];

      end.x = window_min.x;

      back.emplace_back(in[i]);
      back.emplace_back(end);
    }
    // Check from out to out
    else if (std::abs(in[i].x) > std::abs(window_min.x) &&
             std::abs(in[next].x) > std::abs(window_min.x)) {

    }
    // Check from in to in
    else {
      if (state == polygon_state::close) {
        state = polygon_state::open;
        clipped_polygon.emplace_back();
      }

      auto &back = clipped_polygon.back();
      back.emplace_back(in[i]);
      back.emplace_back(in[next]);
    }
  }

  return clipped_polygon;
}

void Polygon::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
                   Vec viewport_max) const {
  cr->set_line_width(width);

  cr->set_source_rgb(colour.r, colour.g, colour.b);

  auto window_min = Vec{-1.0, -1.0};
  auto window_max = Vec{1.0, 1.0};

  std::vector<std::vector<Vec>> clipped_polygon =
      polygon_clipping(points, window_min, window_max);

  double offset = 0.15;
  window_min = Vec{-1.0 - offset, -1.0 - offset};
  window_max = Vec{1.0 + offset, 1.0 + offset};

  for (const auto &clipped_points : clipped_polygon) {
    for (int i = 0; i < clipped_points.size(); i++) {

      Vec transformed_begin;
      Vec transformed_end;

      transformed_begin.x = (clipped_points[i].x - window_min.x) /
                            (window_max.x - window_min.x) *
                            (viewport_max.x - viewport_min.x);
      transformed_begin.y = (1 - (clipped_points[i].y - window_min.y) /
                                     (window_max.y - window_min.y)) *
                            (viewport_max.y - viewport_min.y);

      transformed_end.x =
          (clipped_points[(i + 1) % clipped_points.size()].x - window_min.x) /
          (window_max.x - window_min.x) * (viewport_max.x - viewport_min.x);
      transformed_end.y =
          (1 -
           (clipped_points[(i + 1) % clipped_points.size()].y - window_min.y) /
               (window_max.y - window_min.y)) *
          (viewport_max.y - viewport_min.y);

      cr->move_to(transformed_begin.x, transformed_begin.y);
      cr->line_to(transformed_end.x, transformed_end.y);
      cr->stroke();
    }
  }
}

void Polygon::transform(const Matrix &transform) noexcept {
  for (auto &vec : points) {
    vec = multiply(transform, vec);
  }
}

Vec Polygon::mass_centre() noexcept {
  double x_sum = 0.0;
  double y_sum = 0.0;

  for (const auto &point : points) {
    x_sum += point.x;
    y_sum += point.y;
  }

  x_sum /= points.size();
  y_sum /= points.size();

  return Vec{x_sum, y_sum};
}

std::string Polygon::type() const noexcept { return "Polygon"; }

const std::string &Polygon::name() const noexcept { return m_name; }

std::string &Polygon::name() noexcept { return m_name; }

} // namespace cool_gl
