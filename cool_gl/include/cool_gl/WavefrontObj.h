#pragma once

#include "Drawable.h"
#include "Polygon.h"
#include <istream>

namespace cool_gl {

class WavefrontObj : public Drawable {
public:
  Drawable *copy() const noexcept;

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
            Vec viewport_max) const final;

  void transform(const Matrix &transform) noexcept;

  Vec mass_centre() noexcept;

  std::string type() const noexcept;

  const std::string &name() const noexcept;
  std::string &name() noexcept;

  std::vector<Vec> vertex_data;

  std::vector<std::tuple<int, int, int>> faces;

  std::string m_name;
};

WavefrontObj make_polygon_from_obj(const std::istream &input) noexcept;
}
