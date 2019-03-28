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
  const char *gtk_builder_glade_file = "cool_app_window_design.glade";

  Glib::RefPtr<Gtk::Application> gtk_application;
  Glib::RefPtr<Gtk::Builder> gtk_builder;

  const char *cool_main_application_window_id = "cool_main_application_window";
  Gtk::ApplicationWindow *cool_main_application_window;

  const char *cool_drawing_area_id = "cool_drawing_area";
  Gtk::DrawingArea *cool_drawing_area;

  const char *cool_main_entry_id = "cool_main_entry";
  Gtk::Entry *cool_main_entry;

  const char *cool_main_entry_button_id = "cool_main_entry_button";
  Gtk::Button *cool_main_entry_button;

  const char *cool_display_file_tree_view_id = "cool_display_file_tree_view";
  Gtk::TreeView *cool_display_file_tree_view;

  const char *cool_navigation_zoom_spin_button_id =
      "cool_navigation_zoom_spin_button";
  Gtk::SpinButton *cool_navigation_zoom_spin_button;

  const char *cool_navigation_button_left_id = "cool_navigation_button_left";
  Gtk::Button *cool_navigation_button_left;

  const char *cool_navigation_button_down_id = "cool_navigation_button_down";
  Gtk::Button *cool_navigation_button_down;

  const char *cool_navigation_button_up_id = "cool_navigation_button_up";
  Gtk::Button *cool_navigation_button_up;

  const char *cool_navigation_button_right_id = "cool_navigation_button_right";
  Gtk::Button *cool_navigation_button_right;

  const double ZOOM_FACTOR = 1.0;
  const double MOVE_FACTOR = 1.0;

  int zoom_factor = 0;

  cool_gl::Vec window_begin = {0.0, 0.0, 1.0};
  cool_gl::Vec window_end = {40.0, 40.0, 1.0};

  std::string create_drawable_entrie_string;

  std::vector<std::unique_ptr<cool_gl::Drawable>> drawable_vector;

  ModelColumns my_columns;
  Glib::RefPtr<Gtk::ListStore> object_list;

  CoolApp();

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
