#include <cool_gl/Window.h>
#include <iostream>

namespace cool_gl {

Window::Window(double height, double width, Vec centre, Vec view_up,
               Vec view_right) noexcept
    : height{height}, width{width}, centre{centre}, view_up{view_up},
      view_right{view_right} {}

void Window::move_up(double step) noexcept {
  centre.x = centre.x + view_up.x * step;
  centre.y = centre.y + view_up.y * step;
  std::cout << "centre: " << centre.x << " " << centre.y << std::endl;
}

void Window::move_right(double step) noexcept {
  centre.x = centre.x + view_right.x * step;
  centre.y = centre.y + view_right.y * step;
  std::cout << "centre: " << centre.x << " " << centre.y << std::endl;
}

Window::display_file_type Window::create_normalized_display_file(
    const Window::display_file_type &display_file) noexcept {

  auto normalized_display_file = Window::display_file_type{};

  auto world_to_window_centre =
      cool_gl::create_translate_transform(centre.x, centre.y, centre.z);

  auto world_from_window_centre = cool_gl::create_translate_transform(
      -1.0 * centre.x, -1.0 * centre.y, -1.0 * centre.z);

  auto normalize_coordinates_transform =
      cool_gl::create_scale_transform(2.0 / width, 2.0 / height, 1.0);

  auto world_up = Vec{0.0, 1.0};
  auto window_angle = angle(view_up, world_up);

  auto rotate_world_to_window_up = create_rotate_transform(-1.0 * window_angle);
  if (view_up.x <= 0.0) {
    rotate_world_to_window_up = create_rotate_transform(window_angle);
  }

  auto final_transform = cool_gl::multiply(world_to_window_centre,
                                           normalize_coordinates_transform);
  final_transform =
      cool_gl::multiply(final_transform, world_from_window_centre);
  final_transform =
      cool_gl::multiply(final_transform, rotate_world_to_window_up);

  for (const auto &drawable : display_file) {
    auto copy = drawable->copy();

    copy->transform(final_transform);

    normalized_display_file.emplace_back(copy);
  }
  return normalized_display_file;
}

} // namespace cool_gl
