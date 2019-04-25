#include "clipping.h"

namespace cool_gl {

// Region Codes for Cohen Sutherland Clipping
constexpr int INSIDE = 0; // 0000
constexpr int LEFT = 1;   // 0001
constexpr int RIGHT = 2;  // 0010
constexpr int BOTTOM = 4; // 0100
constexpr int TOP = 8;    // 1000

int check_point_region(double x, double y, Vec window_min, Vec window_max) {
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

std::vector<Vec> cohen_sutherland_clipping(Vec begin, Vec end,
                                           const Vec &window_min,
                                           const Vec &window_max) {
  int begin_code = check_point_region(begin.x, begin.y, window_min, window_max);
  int end_code = check_point_region(end.x, end.y, window_min, window_max);

  while (true) {
    if (begin_code == 0 && end_code == 0) {
      return {begin, end};
    } else if (begin_code & end_code) {
      return {};
    } else {
      int outside_region_code;
      double new_x, new_y;

      if (begin_code != 0) {
        outside_region_code = begin_code;
      } else {
        outside_region_code = end_code;
      }

      if (outside_region_code & TOP) {
        new_x = begin.x + (end.x - begin.x) * (window_max.y - begin.y) /
                              (end.y - begin.y);
        new_y = window_max.y;
      } else if (outside_region_code & BOTTOM) {
        new_x = begin.x + (end.x - begin.x) * (window_min.y - begin.y) /
                              (end.y - begin.y);
        new_y = window_min.y;
      } else if (outside_region_code & RIGHT) {
        new_x = window_max.x;
        new_y = begin.y + (end.y - begin.y) * (window_max.x - begin.x) /
                              (end.x - begin.x);
      } else if (outside_region_code & LEFT) {
        new_x = window_min.x;
        new_y = begin.y + (end.y - begin.y) * (window_min.x - begin.x) /
                              (end.x - begin.x);
      }

      // Replace point outside clipping area by new x and y (intersection)
      if (outside_region_code == begin_code) {
        begin = {new_x, new_y};
        begin_code =
            check_point_region(begin.x, begin.y, window_min, window_max);
      } else {
        end = {new_x, new_y};
        end_code = check_point_region(end.x, end.y, window_min, window_max);
      }
    }
  }

  return {begin, end};
}

Vec find_intersection_x(const Vec &begin, const Vec &end, double x) {
  double m = (end.y - begin.y) / (end.x - begin.x);
  double y = m * (x - begin.x) + begin.y;
  return {x, y};
}

Vec find_intersection_y(const Vec &begin, const Vec &end, double y) {
  double m = (end.y - begin.y) / (end.x - begin.x);
  double x = ((y - begin.y) / m) + begin.x;
  return {x, y};
}

std::vector<Vec> sutherland_hodgman_clipping(const std::vector<Vec> &polygon,
                                             const Vec &window_min,
                                             const Vec &window_max) {
  std::vector<Vec> cliping_edges = {window_min,
                                    {window_min.x, window_max.y},
                                    window_max,
                                    {window_max.x, window_min.y}};

  std::vector<Vec> in = polygon;
  std::vector<Vec> out = {};
  // First clipping edge
  for (int i = 0; i < in.size(); i++) {
    int next = (i + 1) % in.size();

    // Check in in
    if (in[i].x >= window_min.x && in[next].x >= window_min.x) {
      out.emplace_back(in[next]);
    }
    // Check in out
    else if (in[i].x >= window_min.x && in[next].x < window_min.x) {

      auto intersect_point = find_intersection_x(in[i], in[next], window_min.x);

      out.emplace_back(intersect_point);
    }
    // Check out in
    else if (in[i].x < window_min.x && in[next].x >= window_min.x) {

      auto intersect_point = find_intersection_x(in[i], in[next], window_min.x);

      out.emplace_back(intersect_point);
      out.emplace_back(in[next]);
    }
  }
  in = out;
  out.clear();
  // Second clipping edge
  for (int i = 0; i < in.size(); i++) {
    int next = (i + 1) % in.size();

    // Check in in
    if (in[i].y <= window_max.y && in[next].y <= window_max.y) {
      out.emplace_back(in[next]);
    }
    // Check in out
    else if (in[i].y <= window_max.y && in[next].y > window_max.y) {

      auto intersect_point = find_intersection_y(in[i], in[next], window_max.y);

      out.emplace_back(intersect_point);
    }
    // Check out in
    else if (in[i].y > window_max.y && in[next].y <= window_max.y) {

      auto intersect_point = find_intersection_y(in[i], in[next], window_max.y);

      out.emplace_back(intersect_point);
      out.emplace_back(in[next]);
    }
  }
  in = out;
  out.clear();
  // Third clipping edge
  for (int i = 0; i < in.size(); i++) {
    int next = (i + 1) % in.size();

    // Check in in
    if (in[i].x <= window_max.x && in[next].x <= window_max.x) {
      out.emplace_back(in[next]);
    }
    // Check in out
    else if (in[i].x <= window_max.x && in[next].x > window_max.x) {

      auto intersect_point = find_intersection_x(in[i], in[next], window_max.x);

      out.emplace_back(intersect_point);
    }
    // Check out in
    else if (in[i].x > window_max.x && in[next].x <= window_max.x) {

      auto intersect_point = find_intersection_x(in[i], in[next], window_max.x);

      out.emplace_back(intersect_point);
      out.emplace_back(in[next]);
    }
  }
  in = out;
  out.clear();
  // Fourth clipping edge
  for (int i = 0; i < in.size(); i++) {
    int next = (i + 1) % in.size();

    // Check in in
    if (in[i].y >= window_min.y && in[next].y >= window_min.y) {
      out.emplace_back(in[next]);
    }
    // Check in out
    else if (in[i].y >= window_min.y && in[next].y < window_min.y) {

      auto intersect_point = find_intersection_y(in[i], in[next], window_min.y);

      out.emplace_back(intersect_point);
    }
    // Check out in
    else if (in[i].y < window_min.y && in[next].y >= window_min.y) {

      auto intersect_point = find_intersection_y(in[i], in[next], window_min.y);

      out.emplace_back(intersect_point);
      out.emplace_back(in[next]);
    }
  }

  return out;
}

} // namespace cool_gl
