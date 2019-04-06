#include <cool_gl/Window.h>

namespace cool_gl {

Window::Window(double height, double width, Vec centre, Vec view_up) noexcept
    : height{height}, width{width}, centre{centre}, view_up{view_up} {}

Window::display_file_type Window::create_normalized_display_file(
    const Window::display_file_type &display_file) noexcept {

  auto normalized_display_file = Window::display_file_type{};
  for (const auto &drawable : display_file) {
    auto copy = drawable->copy();

    auto translate_to_window_centre_transform =
        create_translate_transform(centre.x, centre.y, centre.z);

    auto scale_transform =
        create_scale_transform(1.0 / width, 1.0 / height, 1.0);

    auto result_transform =
        multiply(translate_to_window_centre_transform, scale_transform);

    copy->transform(result_transform);

    normalized_display_file.emplace_back(copy);
  }
  return normalized_display_file;
}

} // namespace cool_gl
