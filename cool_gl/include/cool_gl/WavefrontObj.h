#pragma once

#include "Polygon.h"
#include <istream>

namespace cool_gl {

Polygon make_polygon_from_obj(const std::istream &input) noexcept;
}
