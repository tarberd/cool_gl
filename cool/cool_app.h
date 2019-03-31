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

  const char *cool_main_entry_text_view_output_id =
      "cool_main_entry_text_view_output";
  Gtk::TextView *cool_main_entry_text_view_output;

  const char *cool_display_file_tree_view_id = "cool_display_file_tree_view";
  Gtk::TreeView *cool_display_file_tree_view;

  const char *cool_navigation_zoom_spin_button_id =
      "cool_navigation_zoom_spin_button";
  Gtk::SpinButton *cool_navigation_zoom_spin_button;

  const char *cool_navigation_zoom_in_button_id =
      "cool_navigation_zoom_in_button";
  Gtk::Button *cool_navigation_zoom_in_button;

  const char *cool_navigation_zoom_out_button_id =
      "cool_navigation_zoom_out_button";
  Gtk::Button *cool_navigation_zoom_out_button;

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

  cool_gl::Vec window_begin = {-20.0, -20.0};
  cool_gl::Vec window_end = {20.0, 20.0};

  std::string create_drawable_entrie_string;

  std::vector<std::unique_ptr<cool_gl::Drawable>> drawable_vector;

  std::map<std::string, cool_gl::Matrix> name_to_transform;

  ModelColumns my_columns;
  Glib::RefPtr<Gtk::ListStore> object_list;

  CoolApp(int argc, char **argv);

  bool cool_drawing_area_draw(const Cairo::RefPtr<Cairo::Context> &cr);
  void cool_navigation_button_down_clicked();
  void cool_navigation_button_up_clicked();
  void cool_navigation_button_right_clicked();
  void cool_navigation_button_left_clicked();
  void cool_navigation_zoom_out_button_clicked();
  void cool_navigation_zoom_in_button_clicked();
  void cool_navigation_zoom_spin_button_changed();
  void cool_main_entry_changed();
  void cool_main_entry_button_clicked();
  int run();
};

} // namespace cool_app
