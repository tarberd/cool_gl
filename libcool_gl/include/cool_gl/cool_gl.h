#pragma once

#include <gtkmm.h>

namespace cool_gl {

struct Vec3 {
  double x;
  double y;
  double z;

  Vec3() = default;
  Vec3(const Vec3 &) = default;
  Vec3(Vec3 &&) = default;
  Vec3(double x, double y, double z) : x{x}, y{y}, z{z} {}
};

struct Colour {
  double r{0.0};
  double g{0.0};
  double b{0.0};

  Colour() = default;
  Colour(const Colour &) = default;
  Colour(Colour &&) = default;
  Colour(double r, double g, double b) : r{r}, g{g}, b{b} {}
};

struct Drawable {
  virtual ~Drawable();
  virtual void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec3 window_min, Vec3 window_max, Vec3 viewport_min, Vec3 viewport_max) const = 0;
  virtual std::string type() const noexcept = 0;
  virtual const std::string &name() const noexcept = 0;
  virtual std::string &name() noexcept = 0;
};

struct Point : public Drawable {
  Vec3 position;
  Colour colour;
  std::string m_name;

  Point() = delete;
  Point(const Point &) = default;
  Point(Point &&) = default;

  template <class A, class B, class C>
  Point(A &&position, B &&colour, C &&name) noexcept
      : position{std::forward<A>(position)}, colour{std::forward<B>(colour)},
        m_name{std::forward<C>(name)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec3 window_min, Vec3 window_max, Vec3 viewport_min, Vec3 viewport_max) const final;

  std::string type() const noexcept final;

  const std::string &name() const noexcept final;
  std::string & name() noexcept final;
};

struct Line : public Drawable {
  Vec3 begin;
  Vec3 end;

  Colour colour;

  std::string m_name;

  Line() = default;
  Line(const Line &) = default;
  Line(Line &&) = default;

  template <class A, class B, class C, class D>
  Line(A &&begin, B &&end, C &&colour, D &&name) noexcept
      : begin{std::forward<A>(begin)}, end{std::forward<B>(end)},
        colour{std::forward<C>(colour)}, m_name{std::forward<D>(name)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec3 window_min, Vec3 window_max, Vec3 viewport_min, Vec3 viewport_max) const final;

  std::string type() const noexcept final;

  const std::string &name() const noexcept final;
  std::string &name() noexcept final;
};

struct Polygon : public Drawable {
  std::vector<Vec3> points;

  Colour colour;

  std::string m_name;

  Polygon() = delete;
  Polygon(const Polygon &) = default;
  Polygon(Polygon &&) = default;

  template <class A, class B, class C>
  Polygon(A &&points, B &&colour, C &&name) noexcept
      : points{std::forward<A>(points)}, colour{std::forward<B>(colour)},
        m_name{std::forward<C>(name)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec3 window_min, Vec3 window_max, Vec3 viewport_min, Vec3 viewport_max) const final;

  std::string type() const noexcept final;

  const std::string & name() const noexcept final;
  std::string & name() noexcept final;
};

} // namespace cool_gl
