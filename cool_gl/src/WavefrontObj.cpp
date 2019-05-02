#include <cool_gl/WavefrontObj.h>

namespace cool_gl {

Polygon make_polygon_from_obj(const std::istream &input) noexcept {
  return Polygon{std::vector<Vec>{}, Colour{0.0, 0.0, 0.0}, ""};
}

} // namespace cool_gl
