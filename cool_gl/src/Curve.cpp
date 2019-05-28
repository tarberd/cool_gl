#include "clipping.h"
#include <cool_gl/Curve.h>
#include <iostream>

namespace cool_gl {

Drawable *Curve::copy() const noexcept { return new Curve{*this}; }

std::vector<Vec> Curve::bezier_curve() const noexcept {

  std::vector<Vec> result;

  result.emplace_back(begin);
  result.emplace_back((begin.x + end.x + 0.5) / 2, (begin.y + end.y + 0.5) / 2);
  result.emplace_back(end);

  // TODO: make real curve points

  return result;
}

Vec viewport_transform(const Vec &in, const Vec &window_min,
                       const Vec &window_max, const Vec &viewport_min,
                       const Vec &viewport_max) {
  Vec transformed;

  transformed.x = (in.x - window_min.x) / (window_max.x - window_min.x) *
                  (viewport_max.x - viewport_min.x);
  transformed.y = (1 - (in.y - window_min.y) / (window_max.y - window_min.y)) *
                  (viewport_max.y - viewport_min.y);
  return transformed;
}

void Curve::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
                Vec viewport_max) const {

  cr->set_line_width(width);
  cr->set_source_rgb(colour.r, colour.g, colour.b);

  auto result = bezier_curve();


  auto window_min = Vec{-1.0, -1.0};
  auto window_max = Vec{1.0, 1.0};

  auto clipped_result = std::vector<Vec>{};

  for (int i = 1; i < result.size(); i++) {
    auto vector = cohen_sutherland_clipping(result[i - 1], result[i],
                                            window_min, window_max);

    for (const auto &vec : vector) {
      clipped_result.emplace_back(vec);
    }
  }

  double offset = 0.15;
  window_min = Vec{-1.0 - offset, -1.0 - offset};
  window_max = Vec{1.0 + offset, 1.0 + offset};

  auto transformed_result = std::vector<Vec>{};

  for (const Vec &vec : clipped_result) {
    transformed_result.emplace_back(viewport_transform(
        vec, window_min, window_max, viewport_min, viewport_max));
  }

  for (int i = 0; i < transformed_result.size(); i += 2) {
    cr->move_to(transformed_result[i].x, transformed_result[i].y);
    cr->line_to(transformed_result[i + 1].x, transformed_result[i + 1].y);
    cr->stroke();
  }
}

void Curve::transform(const Matrix &transform) noexcept {
  begin = multiply(transform, begin);
  end = multiply(transform, end);
}

Vec Curve::mass_centre() noexcept {
  double x_centre = (begin.x + end.x) / 2.0;
  double y_centre = (begin.y + end.y) / 2.0;

  return Vec{x_centre, y_centre};
}

std::string Curve::type() const noexcept { return "Curve"; }

const std::string &Curve::name() const noexcept { return m_name; }

std::string &Curve::name() noexcept { return m_name; }

} // namespace cool_gl
