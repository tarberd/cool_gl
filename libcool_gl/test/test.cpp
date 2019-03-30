#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <cool_gl/cool_gl.h>

using namespace cool_gl;

void test_vec_matrix_multiply(Matrix transform, Vec in, Vec expected) {
  auto result = multiply(transform, in);

  REQUIRE(result[0] == expected.x);
  REQUIRE(result[1] == expected.y);
  REQUIRE(result[2] == expected.z);
  REQUIRE(result[3] == expected.a);
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

TEST_CASE("Test translation transform", "[cool_gl]") {
  auto v = Vec{3.0, -17.0, 5.0};
  auto m = create_translate_transform(5.0, 2.0, -3.0);

  test_vec_matrix_multiply(m, v, {8.0, -15.0, 2.0});
}
