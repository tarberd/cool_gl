#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <cool_gl/cool_gl.h>
#include <iostream>

#include <cmath>

using namespace cool_gl;

double pi() { return std::atan(1) * 4; };

void test_vec_matrix_multiply(Matrix transform, Vec in, Vec expected) {
  auto result = multiply(transform, in);

  REQUIRE(result[0] == Approx(expected.x).margin(1e-10));
  REQUIRE(result[1] == Approx(expected.y).margin(1e-10));
  REQUIRE(result[2] == Approx(expected.z).margin(1e-10));
  REQUIRE(result[3] == Approx(expected.a).margin(1e-10));
}

void test_matrix_matrix_multiply(Matrix left, Matrix right, Matrix expected) {
  auto result = multiply(left, right);

  for (int i = 0; i < left.size(); i++) {
    for (int j = 0; j < left.size(); j++) {
      DYNAMIC_SECTION("Looped section i = " << i << " j = " << j) {
        REQUIRE(result[i][j] == expected[i][j]);
      }
    }
  }
}

TEST_CASE("Vector Matrix multiplication", "[cool_gl]") {
  auto v = Vec{1.0, 1.0, 1.0, 1.0};
  auto m = Matrix{{{1.0, 0.0, 0.0, 0.0},
                   {0.0, 1.0, 0.0, 0.0},
                   {0.0, 0.0, 1.0, 0.0},
                   {0.0, 0.0, 0.0, 1.0}}};

  test_vec_matrix_multiply(m, v, {1.0, 1.0, 1.0, 1.0});

  v = Vec{1.0, 1.0, 1.0, 1.0};
  m = Matrix{{{2.0, 0.0, 0.0, 0.0},
              {0.0, 2.0, 0.0, 0.0},
              {0.0, 0.0, 1.0, 0.0},
              {0.0, 0.0, 0.0, 1.0}}};

  test_vec_matrix_multiply(m, v, {2.0, 2.0, 1.0, 1.0});

  v = Vec{-1.0, -1.0, 1.0, 1.0};
  m = Matrix{{{1.0, 0.0, 0.0, 1.0},
              {0.0, 1.0, 0.0, 1.0},
              {0.0, 0.0, 1.0, 0.0},
              {0.0, 0.0, 0.0, 1.0}}};

  test_vec_matrix_multiply(m, v, {0.0, 0.0, 1.0, 1.0});
}

TEST_CASE("Test transform", "[cool_gl]") {
  auto v = Vec{3.0, -17.0, 5.0};
  auto m = create_translate_transform(5.0, 2.0, -3.0);

  test_vec_matrix_multiply(m, v, {8.0, -15.0, 2.0});

  v = Vec{0.0, 0.0, 0.0};
  m = create_scale_transform(2.0, 2.0, 2.0);

  test_vec_matrix_multiply(m, v, {0.0, 0.0, 0.0});

  v = Vec{1.0, 1.0, 1.0};
  m = create_scale_transform(2.0, 3.0, 4.0);

  test_vec_matrix_multiply(m, v, {2.0, 3.0, 4.0});

  v = Vec{1.0, 2.0, -3.0};
  m = create_scale_transform(2.0, 3.0, 4.0);

  test_vec_matrix_multiply(m, v, {2.0, 6.0, -12.0});
}

TEST_CASE("Test matrix matrix multiplication", "[cool_gl]") {
  auto m1 = create_translate_transform(5.0, 2.0, -3.0);
  auto m2 = create_translate_transform(5.0, 2.0, -3.0);
  auto expected = Matrix{{{1.0, 0.0, 0.0, 10.0},
                          {0.0, 1.0, 0.0, 4.0},
                          {0.0, 0.0, 1.0, -6.0},
                          {0.0, 0.0, 0.0, 1.0}}};

  test_matrix_matrix_multiply(m1, m2, expected);
}

TEST_CASE("Test transform operation compose", "[cool_gl]") {

  auto translate = create_translate_transform(1.0, 1.0, 0.0);

  auto rotate = create_rotate_transform(pi());

  auto vec = Vec{1.0, 0.0, 0.0, 1.0};

  auto expected = Vec{0.0, 1.0, 0.0, 1.0};

  auto final_transform = multiply(rotate, translate);

  test_vec_matrix_multiply(final_transform, vec, expected);

  auto translate2 = create_translate_transform(-1.0, -1.0, 0.0);

  auto transform = multiply(translate2, rotate);

  vec = Vec{2.0, 1.0, 0.0, 1.0};

  expected = Vec{-1.0, 0.0, 0.0, 1.0};

  test_vec_matrix_multiply(transform, vec, expected);

  vec = Vec{2.0, 1.0, 0.0, 1.0};
  expected = Vec{0.0, 1.0, 0.0, 1.0};

  transform = multiply(translate2, rotate);
  transform = multiply(transform, translate);

  test_vec_matrix_multiply(transform, vec, expected);
}
