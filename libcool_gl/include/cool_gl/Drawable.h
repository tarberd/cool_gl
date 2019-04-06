#pragma once

#include <cool_gl/Matrix.h>
#include <cool_gl/Vec.h>
#include <gtkmm.h>
#include <string>

namespace cool_gl {

struct Drawable {
  virtual ~Drawable() = default;

  virtual Drawable *copy() const noexcept = 0;

  virtual void draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec window_min,
                    Vec window_max, Vec viewport_min,
                    Vec viewport_max) const = 0;

  virtual void transform(const Matrix &transform) noexcept = 0;

  virtual Vec mass_centre() noexcept = 0;

  virtual std::string type() const noexcept = 0;

  virtual const std::string &name() const noexcept = 0;
  virtual std::string &name() noexcept = 0;
};

} // namespace cool_gl
