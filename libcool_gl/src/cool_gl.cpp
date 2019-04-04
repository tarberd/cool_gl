#include <cmath>
#include <cool_gl/cool_gl.h>
#include <exception>

namespace cool_gl {

const double &Vec::operator[](int index) const {
  switch (index) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  case 3:
    return a;
  default:
    throw std::out_of_range("Vec::operator[] out off bounds");
  }
}

double &Vec::operator[](int index) {
  switch (index) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  case 3:
    return a;
  default:
    throw std::out_of_range("Vec::operator[] out off bounds");
  }
}

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

Drawable::~Drawable(){};

void Line::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec window_min,
                Vec window_max, Vec viewport_min, Vec viewport_max) const {
  cr->set_line_width(10.0);

  Vec transformed_begin;
  Vec transformed_end;

  transformed_begin.x = (begin.x - window_min.x)/(window_max.x - window_min.x)*(viewport_max.x - viewport_min.x);
  transformed_begin.y = (1 - (begin.y - window_min.y)/(window_max.y - window_min.y))*(viewport_max.y - viewport_min.y);

  transformed_end.x = (end.x - window_min.x)/(window_max.x - window_min.x)*(viewport_max.x - viewport_min.x);
  transformed_end.y = (1 - (end.y - window_min.y)/(window_max.y - window_min.y))*(viewport_max.y - viewport_min.y);

  cr->set_source_rgb(colour.r, colour.g, colour.b);
  cr->move_to(transformed_begin.x, transformed_begin.y);
  cr->line_to(transformed_end.x, transformed_end.y);
  cr->stroke();
}

void Line::transform(const Matrix &transform) noexcept {
  begin = multiply(transform, begin);
  end = multiply(transform, end);
}

Vec Line::mass_centre() noexcept {
  double x_centre = (begin.x + end.x) / 2.0;
  double y_centre = (begin.y + end.y) / 2.0;

  return Vec{x_centre, y_centre};
}

std::string Line::type() const noexcept {
  return "Line";
}

const std::string & Line::name() const noexcept {
    return m_name;
}

std::string & Line::name() noexcept {
    return m_name;
}

void Point::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec window_min,
                 Vec window_max, Vec viewport_min, Vec viewport_max) const {
  cr->set_line_width(10.0);

  Vec transformed_point;

  transformed_point.x = (position.x - window_min.x)/(window_max.x - window_min.x)*(viewport_max.x - viewport_min.x);
  transformed_point.y = (1 - (position.y - window_min.y)/(window_max.y - window_min.y))*(viewport_max.y - viewport_min.y);

  cr->set_source_rgb(colour.r, colour.g, colour.b);
  cr->move_to(transformed_point.x, transformed_point.y);
  cr->arc(transformed_point.x, transformed_point.y, 5.0, 0.0, 2*M_PI);
  cr->fill();
}

void Point::transform(const Matrix &transform) noexcept {
  position = multiply(transform, position);
}

Vec Point::mass_centre() noexcept { return position; }

std::string Point::type() const noexcept { return "Point"; }

const std::string &Point::name() const noexcept { return m_name; }

std::string &Point::name() noexcept { return m_name; }

void Polygon::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec window_min,
                   Vec window_max, Vec viewport_min, Vec viewport_max) const {
  cr->set_line_width(10.0);

  cr->set_source_rgb(colour.r, colour.g, colour.b);

  for(int i = 0; i < points.size(); i++) {

    Vec transformed_begin;
    Vec transformed_end;

    transformed_begin.x = (points[i].x - window_min.x) /
                          (window_max.x - window_min.x) *
                          (viewport_max.x - viewport_min.x);
    transformed_begin.y =
        (1 - (points[i].y - window_min.y) / (window_max.y - window_min.y)) *
        (viewport_max.y - viewport_min.y);

    transformed_end.x = (points[(i + 1) % points.size()].x - window_min.x) /
                        (window_max.x - window_min.x) *
                        (viewport_max.x - viewport_min.x);
    transformed_end.y =
        (1 - (points[(i + 1) % points.size()].y - window_min.y) /
                 (window_max.y - window_min.y)) *
        (viewport_max.y - viewport_min.y);

    cr->move_to(transformed_begin.x, transformed_begin.y);
    cr->line_to(transformed_end.x, transformed_end.y);
    cr->stroke();
  }
}

void Polygon::transform(const Matrix &transform) noexcept {
  for (auto &vec : points) {
    vec = multiply(transform, vec);
  }
}

Vec Polygon::mass_centre() noexcept {
  double x_sum = 0.0;
  double y_sum = 0.0;

  for (const auto &point : points) {
    x_sum += point.x;
    y_sum += point.y;
  }

  x_sum /= points.size();
  y_sum /= points.size();

  return Vec{x_sum, y_sum};
}

std::string Polygon::type() const noexcept { return "Polygon"; }

const std::string &Polygon::name() const noexcept { return m_name; }

std::string &Polygon::name() noexcept { return m_name; }

} // namespace cool_gl
