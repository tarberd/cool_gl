#include <cool_gl/cool_gl.h>

namespace cool_gl {

void Line::draw(const Cairo::RefPtr<Cairo::Context> &cr) const {
  cr->set_line_width(10.0);

  cr->set_source_rgb(colour.r, colour.g, colour.b);
  cr->move_to(begin.x, begin.y);
  cr->line_to(end.x, end.y);
  cr->stroke();
}

Drawable_type Line::type() const noexcept { return Drawable_type::line; }

} // namespace cool_gl
