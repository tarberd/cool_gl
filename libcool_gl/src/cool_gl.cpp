#include <cool_gl/cool_gl.h>

namespace cool_gl {

Drawable::~Drawable(){};

void Line::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec3 window_min, Vec3 window_max, Vec3 viewport_min, Vec3 viewport_max) const {
  cr->set_line_width(10.0);

  Vec3 transformed_begin;
  Vec3 transformed_end;

  transformed_begin.x = (begin.x - window_min.x)/(window_max.x - window_min.x)*(viewport_max.x - viewport_min.x);
  transformed_begin.y = (1 - (begin.y - window_min.y)/(window_max.y - window_min.y))*(viewport_max.y - viewport_min.y);

  transformed_end.x = (end.x - window_min.x)/(window_max.x - window_min.x)*(viewport_max.x - viewport_min.x);
  transformed_end.y = (1 - (end.y - window_min.y)/(window_max.y - window_min.y))*(viewport_max.y - viewport_min.y);

  cr->set_source_rgb(colour.r, colour.g, colour.b);
  cr->move_to(transformed_begin.x, transformed_begin.y);
  cr->line_to(transformed_end.x, transformed_end.y);
  cr->stroke();
}

Drawable_type Line::type() const noexcept { return Drawable_type::line; }

void Point::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec3 window_min, Vec3 window_max, Vec3 viewport_min, Vec3 viewport_max) const {
  cr->set_line_width(10.0);

  Vec3 transformed_point;

  transformed_point.x = (position.x - window_min.x)/(window_max.x - window_min.x)*(viewport_max.x - viewport_min.x);
  transformed_point.y = (1 - (position.y - window_min.y)/(window_max.y - window_min.y))*(viewport_max.y - viewport_min.y);

  cr->set_source_rgb(colour.r, colour.g, colour.b);
  cr->move_to(transformed_point.x, transformed_point.y);
  cr->arc(transformed_point.x, transformed_point.y, 5.0, 0.0, 2*M_PI);
  cr->fill();
}

Drawable_type Point::type() const noexcept { return Drawable_type::point; }

void Polygon::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec3 window_min, Vec3 window_max, Vec3 viewport_min, Vec3 viewport_max) const {
  cr->set_line_width(10.0);

  cr->set_source_rgb(colour.r, colour.g, colour.b);

  for(int i = 0; i < points.size(); i++) {

      Vec3 transformed_begin;
      Vec3 transformed_end;

      transformed_begin.x = (points[i].x - window_min.x)/(window_max.x - window_min.x)*(viewport_max.x - viewport_min.x);
      transformed_begin.y = (1 - (points[i].y - window_min.y)/(window_max.y - window_min.y))*(viewport_max.y - viewport_min.y);

      transformed_end.x = (points[(i+1)%points.size()].x - window_min.x)/(window_max.x - window_min.x)*(viewport_max.x - viewport_min.x);
      transformed_end.y = (1 - (points[(i+1)%points.size()].y - window_min.y)/(window_max.y - window_min.y))*(viewport_max.y - viewport_min.y);

      cr->move_to(transformed_begin.x, transformed_begin.y);
      cr->line_to(transformed_end.x, transformed_end.y);
      cr->stroke();
  }
}

Drawable_type Polygon::type() const noexcept { return Drawable_type::polygon; }

} // namespace cool_gl
