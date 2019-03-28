#pragma once

#include "model_columns.h"
#include <cool_gl/cool_gl.h>
#include <gtkmm.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace cool_app {

struct CoolApp {
  const double ZOOM_FACTOR = 1.0;
  const double MOVE_FACTOR = 1.0;

  int zoom_factor = 0;

  cool_gl::Vec3 window_begin = {0.0, 0.0, 1.0};
  cool_gl::Vec3 window_end = {40.0, 40.0, 1.0};

  Gtk::Window *glade_window;
  Gtk::Dialog *dialog_add_drawable;

  Gtk::DrawingArea *glade_drawing_area;

  Gtk::Entry *zoom;
  Gtk::Entry *add_drawable_comand_entry;
  std::string create_drawable_entrie_string;

  std::vector<std::unique_ptr<cool_gl::Drawable>> drawable_vector;

  ModelColumns my_columns;
  Gtk::TreeView *object_tree;
  Glib::RefPtr<Gtk::ListStore> object_list;

  bool draw_callback(const Cairo::RefPtr<Cairo::Context> &cr);
  void move_down();
  void move_up();
  void move_right();
  void move_left();
  void out_callback();
  void in_callback();
  void zoom_value();
  void create_drawable_entry_value();
  void show_add_drawable_dialog();
  void create_drawable_from_entry();
  int run(int argc, char **argv);
};

} // namespace cool_app
