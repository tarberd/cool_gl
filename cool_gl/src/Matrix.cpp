#include <cmath>
#include <cool_gl/Matrix.h>

namespace cool_gl {

Matrix create_translate_transform(double dx, double dy, double dz) {
  return {{{1.0, 0.0, 0.0, dx},
           {0.0, 1.0, 0.0, dy},
           {0.0, 0.0, 1.0, dz},
           {0.0, 0.0, 0.0, 1.0}}};
}

Matrix create_scale_transform(double sx, double sy, double sz) {
  return {{{sx, 0.0, 0.0, 0.0},
           {0.0, sy, 0.0, 0.0},
           {0.0, 0.0, sz, 0.0},
           {0.0, 0.0, 0.0, 1.0}}};
}

Matrix create_rotate_transform(double rad_x, double rad_y, double rad_z) {
  auto rotate_x = Matrix{{{1.0, 0.0, 0.0, 0.0},
                          {0.0, std::cos(rad_x), std::sin(rad_x), 0.0},
                          {0.0, -1.0 * std::sin(rad_x), std::cos(rad_x), 0.0},
                          {0.0, 0.0, 0.0, 1.0}}};

  auto rotate_y = Matrix{{{std::cos(rad_y), 0.0, -1.0 * std::sin(rad_y), 0.0},
                          {0.0, 1.0, 0.0, 0.0},
                          {std::sin(rad_y), 0.0, std::cos(rad_y), 0.0},
                          {0.0, 0.0, 0.0, 1.0}}};

  auto rotate_z = Matrix{{{std::cos(rad_z), std::sin(rad_z), 0.0, 0.0},
                          {-1.0 * std::sin(rad_z), std::cos(rad_z), 0.0, 0.0},
                          {0.0, 0.0, 1.0, 0.0},
                          {0.0, 0.0, 0.0, 1.0}}};

  return multiply(multiply(rotate_x, rotate_y), rotate_z);
}

Matrix create_rotate_transform(double rad) {
  return {{{std::cos(rad), std::sin(rad), 0.0, 0.0},
           {-1.0 * std::sin(rad), std::cos(rad), 0.0, 0.0},
           {0.0, 0.0, 1.0, 0.0},
           {0.0, 0.0, 0.0, 1.0}}};
}

Matrix multiply(const Matrix &right, const Matrix &left) {
  Matrix result;

  int N = left.size();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      Matrix::value_type::value_type num = 0;
      for (int k = 0; k < N; k++) {
        num += left[i][k] * right[k][j];
      }
      result[i][j] = num;
    }
  }

  return result;
}

Vec multiply(const Matrix &left, const Vec &right) {
  Vec result;
  int N = left.size();
  for (int i = 0; i < N; i++) {
    Matrix::value_type::value_type num = 0;
    for (int j = 0; j < N; j++) {
      num += (left[i][j] * right[j]);
    }
    result[i] = num;
  }
  return result;
}

} // namespace cool_gl
