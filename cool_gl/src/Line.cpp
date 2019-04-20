#include <cool_gl/Line.h>
#include <iostream>

namespace cool_gl {


Drawable *Line::copy() const noexcept { return new Line{*this}; }

// Region Codes for Cohen Sutherland Clipping
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

int check_point_region(double x, double y, Vec window_min, Vec window_max){
  int code = 0;
  if (x < window_min.x)
      code |= LEFT;
  else if (x > window_max.x)
      code |= RIGHT;
  if (y < window_min.y)
      code |= BOTTOM;
  else if (y > window_max.y)
      code |= TOP;

  return code;
}

std::vector<Vec> cohen_sutherland_clipping(Vec begin, Vec end, Vec window_min, Vec window_max){
  int begin_code = check_point_region(begin.x, begin.y, window_min, window_max);
  int end_code = check_point_region(end.x, end.y, window_min, window_max);

  std::vector<Vec> clipped_points = {};

  if(begin_code == 0 && end_code == 0){
    clipped_points = { begin, end };
  } else {
    int outside_region_code;
    double new_x, new_y;

    if(begin_code != 0){
      outside_region_code = begin_code;
    } else {
      outside_region_code = end_code;
    }

    if (outside_region_code & TOP){
      new_x = begin.x + (end.x - begin.x) * (window_max.y - begin.y) / (end.y - begin.y);
      new_y = window_max.y;
    }
    else if (outside_region_code & BOTTOM){
      new_x = begin.x + (end.x - begin.x) * (window_min.y - begin.y) / (end.y - begin.y);
      new_y = window_min.y;
    } else if (outside_region_code & RIGHT){
      new_x = window_max.x;
      new_y = begin.y + (end.y - begin.y) * (window_max.x - begin.x) / (end.x - begin.x);
    } else if (outside_region_code & LEFT){
      new_x = window_min.x;
      new_y = begin.y + (end.y - begin.y) * (window_min.x - begin.x) / (end.x - begin.x);
    }

    // Replace point outside clipping area by new x and y (intersection)
    if(outside_region_code == begin_code){
      begin.x = new_x;
      begin.y = new_y;

      clipped_points = { begin, end };
    } else {
      end.x = new_x;
      end.y = new_y;

      clipped_points = { begin, end };
    }
  }

  return clipped_points;
}


void Line::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
                Vec viewport_max) const {
  cr->set_line_width(width);

  Vec transformed_begin;
  Vec transformed_end;

  double offset = 0.15;
  auto window_min = Vec{-1.0 - offset, -1.0 - offset};
  auto window_max = Vec{1.0 + offset, 1.0 + offset};

  // TODO: Change this to a function that will choose between:
  // Cohen-Sutherland and a second algorithm
  std::vector<Vec> clipped_points = cohen_sutherland_clipping(begin, end, window_min, window_max);;

  if(!clipped_points.empty()){
    transformed_begin = clipped_points[0];
    transformed_end = clipped_points[1];

    transformed_begin.x = (begin.x - window_min.x) /
                          (window_max.x - window_min.x) *
                          (viewport_max.x - viewport_min.x);
    transformed_begin.y =
        (1 - (begin.y - window_min.y) / (window_max.y - window_min.y)) *
        (viewport_max.y - viewport_min.y);

    transformed_end.x = (end.x - window_min.x) / (window_max.x - window_min.x) *
                        (viewport_max.x - viewport_min.x);
    transformed_end.y =
        (1 - (end.y - window_min.y) / (window_max.y - window_min.y)) *
        (viewport_max.y - viewport_min.y);

    cr->set_source_rgb(colour.r, colour.g, colour.b);
    cr->move_to(transformed_begin.x, transformed_begin.y);
    cr->line_to(transformed_end.x, transformed_end.y);
    cr->stroke();
  }
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

std::string Line::type() const noexcept { return "Line"; }

const std::string &Line::name() const noexcept { return m_name; }

std::string &Line::name() noexcept { return m_name; }

} // namespace cool_gl
