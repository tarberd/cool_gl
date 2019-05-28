#include "clipping.h"
#include <cool_gl/Curve.h>
#include <iostream>

namespace cool_gl {

Drawable *Curve::copy() const noexcept { return new Curve{*this}; }

std::vector<cool_gl::Line> Curve::bezier_curve() {
  double u = 0.0;
  std::vector<cool_gl::Line> lines;
  std::string name = "partial";
  double x, y;

  for(u = 0.0 ; u <= 1.0 ; u += 0.0001)
  {
    x = pow(1-u,3)*this->control_x[0]+3*u*pow(1-u,2)*this->control_x[1]+3*pow(u,2)*(1-u)*this->control_x[2]
         +pow(u,3)*this->control_x[3];
    y = pow(1-u,3)*this->control_y[0]+3*u*pow(1-u,2)*this->control_y[1]+3*pow(u,2)*(1-u)*this->control_y[2]
        +pow(u,3)*this->control_y[3];

    lines.emplace_back(
        new cool_gl::Line{std::move(x), std::move(y),
                          cool_gl::Colour{0.0, 0.0, 0.0}, std::move(name)});
  }

  return lines;
}

void Curve::draw(const Cairo::RefPtr<Cairo::Context> &cr, Vec viewport_min,
                Vec viewport_max) const {
  std::vector<cool_gl::Line> result;
  result = bezier_curve({*this});

  for(int i = 0; i < result.size(); i ++){
    cr->move_to(result.begin.x, result.begin.y);
    cr->line_to(result.end.x, result.end.y);
    cr->stroke();
  }
}

std::string Curve::type() const noexcept { return "Curve"; }

const std::string &Curve::name() const noexcept { return m_name; }

std::string &Curve::name() noexcept { return m_name; }

} // namespace cool_gl
