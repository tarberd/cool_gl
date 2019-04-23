#include <cool_gl/Line.h>
#include <cool_gl/Polygon.h>
#include <iostream>
#include <vector>

namespace cool_gl {

int check_point_region(double x, double y, Vec window_min, Vec window_max);

std::vector<Vec> cohen_sutherland_clipping(Vec begin, Vec end,
                                           const Vec &window_min,
                                           const Vec &window_max);

} // namespace cool_gl
