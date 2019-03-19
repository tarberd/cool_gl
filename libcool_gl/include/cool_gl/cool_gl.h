#pragma once

#include <gtkmm.h>

namespace cool_gl {

struct Vec3 {
  double x;
  double y;
  double z;
};

enum class Drawable_type : int { point, line };

struct Drawable {
public:
  virtual void draw(const Cairo::RefPtr<Cairo::Context> &cr) const = 0;
  virtual Drawable_type type() const noexcept = 0;
};

struct Colour {
  double r;
  double g;
  double b;
};

struct Line : public Drawable {
  Vec3 begin;
  Vec3 end;

  Colour colour;

  Line() = default;
  Line(const Line &) = default;
  Line(Line &&) = default;

  template <class A, class B, class C>
  Line(A &&begin, B &&end, C &&colour)
      : begin{std::forward<A>(begin)}, end{std::forward<B>(end)},
        colour{std::forward<C>(colour)} {}

  void draw(const Cairo::RefPtr<Cairo::Context> &cr) const final;

  Drawable_type type() const noexcept final;
};

} // namespace cool_gl
