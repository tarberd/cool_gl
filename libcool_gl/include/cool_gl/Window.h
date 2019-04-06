#pragma once

#include <cool_gl/Vec.h>
#include <vector>

namespace cool_gl {

struct Window {

  Window() = delete;
  Window(const Window &) = default;
  Window(Window &&) = default;
  operator=(const Window &) = default;
  operator=(Window &&) = default;

  Window(double height, double width, Vec centre, Vec view_up) noexcept;

  double height;
  double width;
  Vec centre;
  Vec view_up;

  std::vector<std::unique_ptr<cool_gl::Drawable>> normalized_display_file;
};

} // namespace cool_gl
