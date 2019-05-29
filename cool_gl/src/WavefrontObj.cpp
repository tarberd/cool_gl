#include <cool_gl/WavefrontObj.h>
#include <iostream>

namespace cool_gl {
Drawable *WavefrontObj::copy() const noexcept {
  return new WavefrontObj{*this};
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

void WavefrontObj::draw(const Cairo::RefPtr<Cairo::Context> &cr,
                        Vec viewport_min, Vec viewport_max) const {

  cr->set_line_width(width);

  cr->set_source_rgb(colour.r, colour.g, colour.b);

  auto window_min = Vec{-1.0, -1.0};
  auto window_max = Vec{1.0, 1.0};

  // std::vector<Vec> clipped_points =
  //     sutherland_hodgman_clipping(points, window_min, window_max);

  double offset = 0.15;
  window_min = Vec{-1.0 - offset, -1.0 - offset};
  window_max = Vec{1.0 + offset, 1.0 + offset};

  for (const auto &face : faces) {
    Vec first =
        viewport_transform(vertex_data[std::get<0>(face) - 1], window_min,
                           window_max, viewport_min, viewport_max);
    Vec middle =
        viewport_transform(vertex_data[std::get<1>(face) - 1], window_min,
                           window_max, viewport_min, viewport_max);
    Vec last =
        viewport_transform(vertex_data[std::get<2>(face) - 1], window_min,
                           window_max, viewport_min, viewport_max);

    cr->move_to(first.x, first.y);
    cr->line_to(middle.x, middle.y);
    cr->stroke();
    cr->move_to(middle.x, middle.y);
    cr->line_to(last.x, last.y);
    cr->stroke();
    cr->move_to(last.x, last.y);
    cr->line_to(first.x, first.y);
    cr->stroke();
  }
}

void WavefrontObj::transform(const Matrix &transform) noexcept {
  for (auto &vec : vertex_data) {
    vec = multiply(transform, vec);
  }
}

Vec WavefrontObj::mass_centre() noexcept {
  double x_sum = 0.0;
  double y_sum = 0.0;
  double z_sum = 0.0;

  for (const auto &point : vertex_data) {
    x_sum += point.x;
    y_sum += point.y;
    z_sum += point.z;
  }

  x_sum /= vertex_data.size();
  y_sum /= vertex_data.size();
  z_sum /= vertex_data.size();

  return Vec{x_sum, y_sum, z_sum};
}

std::string WavefrontObj::type() const noexcept { return "WavefrontObj"; }

const std::string &WavefrontObj::name() const noexcept { return m_name; }

std::string &WavefrontObj::name() noexcept { return m_name; }

void trim_face_index_input(std::string &index_string) {
  if (index_string.find('/') != std::string::npos) {
    index_string.erase(index_string.find('/'));
  }
}

WavefrontObj make_polygon_from_obj(std::istream &input) noexcept {
  std::vector<Vec> vertex_data;
  std::vector<std::tuple<int, int, int>> faces;
  std::string name;

  auto formated_input = std::string{};

  while (input >> formated_input) {
    std::cout << formated_input << std::endl;
    if (formated_input.compare("#") < 0) {
      input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else if (formated_input.compare("v") == 0) {
      double x;
      double y;
      double z;

      input >> x >> y >> z;

      vertex_data.emplace_back(x, y, z);

      input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else if (formated_input.compare("f") == 0) {
      std::string first_index;
      std::string middle_index;
      std::string last_index;

      input >> first_index >> middle_index >> last_index;

      trim_face_index_input(first_index);
      trim_face_index_input(middle_index);
      trim_face_index_input(last_index);

      faces.emplace_back(std::stoi(first_index), std::stoi(middle_index),
                         std::stoi(last_index));
    } else if (formated_input.compare("g") == 0) {
      input >> name;
    } else {
      input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  for (auto &vertex : vertex_data) {
    std::cout << vertex << std::endl;
  }

  return WavefrontObj{std::move(vertex_data), std::move(faces),
                      Colour{0.0, 0.0, 1.0}, std::move(name)};
}

} // namespace cool_gl
