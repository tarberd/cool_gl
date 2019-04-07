#pragma once

#include <cool_gl/Drawable.h>
#include <cool_gl/Vec.h>
#include <memory>
#include <vector>

namespace cool_gl {

struct Drawable;

struct Window {
  using display_file_type = std::vector<std::unique_ptr<cool_gl::Drawable>>;

  Window() = delete;
  Window(const Window &) = default;
  Window(Window &&) = default;
  Window &operator=(const Window &) = default;
  Window &operator=(Window &&) = default;

  Window(double height, double width, Vec centre, Vec view_up) noexcept;

  display_file_type create_normalized_display_file(
      const display_file_type &display_file) noexcept;

  double height;
  double width;
  Vec centre;
  Vec view_up;
};

} // namespace cool_gl
