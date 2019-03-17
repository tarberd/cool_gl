#pragma once

#include <gtkmm.h>

namespace cool_gl {

struct Drawable {
public:
  virtual void draw(const Cairo::RefPtr<Cairo::Context> &cr) const = 0;
};

struct Colour {
  double r;
  double g;
  double b;
};

struct Point {
  double x;
  double y;
};

struct Line : public Drawable {
  Point begin;
  Point end;

  Colour colour;

  Line() = default;
  Line(const Line &) = default;
  Line(Line &&) = default;

  template <class A, class B, class C>
  Line(A &&begin, B &&end, C &&colour)
      : begin{std::forward<A>(begin)}, end{std::forward<B>(end)},
        colour{std::forward<C>(colour)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr) const final;
};

} // namespace cool_gl
