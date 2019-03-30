#pragma once

#include <array>
#include <gtkmm.h>

namespace cool_gl {

struct Colour {
  double r{0.0};
  double g{0.0};
  double b{0.0};

  Colour() = default;
  Colour(const Colour &) = default;
  Colour(Colour &&) = default;
  Colour(double r, double g, double b) : r{r}, g{g}, b{b} {}
};

struct Vec {
  double x;
  double y;
  double z;
  double a;

  Vec() = default;
  Vec(const Vec &) = default;
  Vec &operator=(const Vec &) = default;
  Vec &operator=(Vec &&) = default;
  Vec(Vec &&) = default;
  Vec(double x, double y, double z = 1.0, double a = 1.0)
      : x{x}, y{y}, z{z}, a{a} {}

  const double &operator[](int x) const;
  double &operator[](int x);
};

using Matrix = std::array<std::array<double, 4>, 4>;

Matrix multiply(const Matrix &left, const Matrix &right);

Vec multiply(const Matrix &left, const Vec &right);

Matrix create_translate_transform(double dx, double xy, double dz);
Matrix create_scale_transform(double sx, double sy, double sz);
Matrix create_rotate_transform(double rad);

struct Drawable {
  virtual ~Drawable();

  virtual void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec window_min,
                    Vec window_max, Vec viewport_min,
                    Vec viewport_max) const = 0;

  virtual void transform(const Matrix &transform) noexcept = 0;

  virtual std::string type() const noexcept = 0;

  virtual const std::string &name() const noexcept = 0;
  virtual std::string &name() noexcept = 0;
};

struct Point : public Drawable {
  Vec position;
  Colour colour;
  std::string m_name;

  Point() = delete;
  Point(const Point &) = default;
  Point(Point &&) = default;

  template <class A, class B, class C>
  Point(A &&position, B &&colour, C &&name) noexcept
      : position{std::forward<A>(position)}, colour{std::forward<B>(colour)},
        m_name{std::forward<C>(name)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec window_min,
            Vec window_max, Vec viewport_min, Vec viewport_max) const final;

  void transform(const Matrix &transform) noexcept;

  std::string type() const noexcept final;

  const std::string &name() const noexcept final;
  std::string & name() noexcept final;
};

struct Line : public Drawable {
  Vec begin;
  Vec end;

  Colour colour;

  std::string m_name;

  Line() = default;
  Line(const Line &) = default;
  Line(Line &&) = default;

  template <class A, class B, class C, class D>
  Line(A &&begin, B &&end, C &&colour, D &&name) noexcept
      : begin{std::forward<A>(begin)}, end{std::forward<B>(end)},
        colour{std::forward<C>(colour)}, m_name{std::forward<D>(name)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec window_min,
            Vec window_max, Vec viewport_min, Vec viewport_max) const final;

  void transform(const Matrix &transform) noexcept;

  std::string type() const noexcept final;

  const std::string &name() const noexcept final;
  std::string &name() noexcept final;
};

struct Polygon : public Drawable {
  std::vector<Vec> points;

  Colour colour;

  std::string m_name;

  Polygon() = delete;
  Polygon(const Polygon &) = default;
  Polygon(Polygon &&) = default;

  template <class A, class B, class C>
  Polygon(A &&points, B &&colour, C &&name) noexcept
      : points{std::forward<A>(points)}, colour{std::forward<B>(colour)},
        m_name{std::forward<C>(name)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec window_min,
            Vec window_max, Vec viewport_min, Vec viewport_max) const final;

  void transform(const Matrix &transform) noexcept;

  std::string type() const noexcept final;

  const std::string & name() const noexcept final;
  std::string & name() noexcept final;
};

} // namespace cool_gl
