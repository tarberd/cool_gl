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

std::vector<Vec> sutherland_hodgman_clipping(const std::vector<Vec> &in,
                                             const Vec &window_min,
                                             const Vec &window_max) {
  std::vector<Vec> out = {};
  // std::vector<Vec> cliping_edges = {window_min,
  //                                   {window_min.x, window_max.y},
  //                                   window_max,
  //                                   {window_max.x, window_min.y}};
  // // First clipping edge
  // for (int i = 0; i < in.size(); i++) {
  //   int next = (i + 1) % in.size();
  //
  //   // Check in in
  //   if (in[i].x >= window_min.x && in[next].x >= window_min.x) {
  //     out.emplace_back(in[next]);
  //   }
  //   // Check in out
  //   else if (in[i].x >= window_min.x && in[next].x < window_min.x) {
  //
  //     auto line =
  //         cohen_sutherland_clipping(in[i], in[next], window_min, window_max);
  //
  //     out.emplace_back(line[1]);
  //   }
  //   // Check out in
  //   else if (in[i].x < window_min.x && in[next].x >= window_min.x) {
  //
  //     auto line =
  //         cohen_sutherland_clipping(in[i], in[next], window_min, window_max);
  //
  //     out.emplace_back(line[0]);
  //     out.emplace_back(line[1]);
  //   }
  // }
  // // Second clipping edge
  // for (int i = 0; i < in.size(); i++) {
  //   int next = (i + 1) % in.size();
  //
  //   // Check in in
  //   if (in[i].y >= window_min.y && in[next].y >= window_min.y) {
  //     out.emplace_back(in[next]);
  //   }
  //   // Check in out
  //   else if (in[i].y >= window_min.y && in[next].y < window_min.y) {
  //
  //     auto line =
  //         cohen_sutherland_clipping(in[i], in[next], window_min, window_max);
  //
  //     out.emplace_back(line[1]);
  //   }
  //   // Check out in
  //   else if (in[i].y < window_min.y && in[next].y >= window_min.y) {
  //
  //     auto line =
  //         cohen_sutherland_clipping(in[i], in[next], window_min, window_max);
  //
  //     out.emplace_back(line[0]);
  //     out.emplace_back(line[1]);
  //   }
  // }
  // // Third clipping edge
  // for (int i = 0; i < in.size(); i++) {
  //   int next = (i + 1) % in.size();
  //
  //   // Check in in
  //   if (in[i].x <= window_max.x && in[next].x <= window_max.x) {
  //     out.emplace_back(in[next]);
  //   }
  //   // Check in out
  //   else if (in[i].x <= window_max.x && in[next].x > window_max.x) {
  //
  //     auto line =
  //         cohen_sutherland_clipping(in[i], in[next], window_min, window_max);
  //
  //     out.emplace_back(line[1]);
  //   }
  //   // Check out in
  //   else if (in[i].x > window_max.x && in[next].x <= window_max.x) {
  //
  //     auto line =
  //         cohen_sutherland_clipping(in[i], in[next], window_min, window_max);
  //
  //     out.emplace_back(line[0]);
  //     out.emplace_back(line[1]);
  //   }
  // }
  // // Fourth clipping edge
  // for (int i = 0; i < in.size(); i++) {
  //   int next = (i + 1) % in.size();
  //
  //   // Check in in
  //   if (in[i].y <= window_max.y && in[next].y <= window_max.y) {
  //     out.emplace_back(in[next]);
  //   }
  //   // Check in out
  //   else if (in[i].y <= window_max.y && in[next].y > window_max.y) {
  //
  //     auto line =
  //         cohen_sutherland_clipping(in[i], in[next], window_min, window_max);
  //
  //     out.emplace_back(line[1]);
  //   }
  //   // Check out in
  //   else if (in[i].y > window_max.y && in[next].y <= window_max.y) {
  //
  //     auto line =
  //         cohen_sutherland_clipping(in[i], in[next], window_min, window_max);
  //
  //     out.emplace_back(line[0]);
  //     out.emplace_back(line[1]);
  //   }
  // }
  //
  return out;
}

} // namespace cool_gl
