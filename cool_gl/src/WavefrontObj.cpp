#include <cool_gl/WavefrontObj.h>

namespace cool_gl {
Drawable *WavefrontObj::copy() const noexcept {
  return new WavefrontObj{*this};
}

void WavefrontObj::draw(const Cairo::RefPtr<Cairo::Context> &cr,
                        Vec viewport_min, Vec viewport_max) const {}

void WavefrontObj::transform(const Matrix &transform) noexcept {}

Vec WavefrontObj::mass_centre() noexcept {
  return Vec{0.0, 0.0, 0.0};
}

std::string WavefrontObj::type() const noexcept { return "WavefrontObj"; }

const std::string &WavefrontObj::name() const noexcept { return m_name; }

std::string &WavefrontObj::name() noexcept { return m_name; }

WavefrontObj make_polygon_from_obj(const std::istream &input) noexcept {
  std::vector<Vec> vertex_data;
  std::vector<std::tuple<int, int, int>> faces;
  std::string name;

  return WavefrontObj{};
}

} // namespace cool_gl
