#pragma once

#include <array>
#include <cool_gl/Vec.h>

namespace cool_gl {

using Matrix = std::array<std::array<double, 4>, 4>;

Matrix multiply(const Matrix &left, const Matrix &right);

Vec multiply(const Matrix &left, const Vec &right);

Matrix create_translate_transform(double dx, double xy, double dz);
Matrix create_scale_transform(double sx, double sy, double sz);
Matrix create_rotate_transform(double rad);
Matrix create_rotate_transform(double rad_x, double rad_y, double rad_z);

} // namespace cool_gl
