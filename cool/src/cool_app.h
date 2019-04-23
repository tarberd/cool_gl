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

  const char *cool_navigation_rotate_spin_button_id =
      "cool_navigation_rotate_spin_button";
  Gtk::SpinButton *cool_navigation_rotate_spin_button;

  const char *cool_navigation_rotate_left_button_id =
      "cool_navigation_rotate_left_button";
  Gtk::Button *cool_navigation_rotate_left_button;

  const char *cool_navigation_rotate_right_button_id =
      "cool_navigation_rotate_right_button";
  Gtk::Button *cool_navigation_rotate_right_button;

  const double ZOOM_FACTOR = 1.0;
  const double MOVE_FACTOR = 0.05;

  double zoom_factor = 0;
  double rotate_factor = 0;

  std::string create_drawable_entrie_string;

  cool_gl::Window window{100, 100, {0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}};
  std::vector<std::unique_ptr<cool_gl::Drawable>> display_file;

  ModelColumns my_columns;
  Glib::RefPtr<Gtk::ListStore> object_list;

  CoolApp(int argc, char **argv);

  bool cool_drawing_area_draw(const Cairo::RefPtr<Cairo::Context> &cr);
  void cool_navigation_move_down_button_clicked();
  void cool_navigation_move_up_button_clicked();
  void cool_navigation_move_right_button_clicked();
  void cool_navigation_move_left_button_clicked();
  void cool_navigation_zoom_out_button_clicked();
  void cool_navigation_zoom_in_button_clicked();
  void cool_navigation_zoom_spin_button_changed();
  void cool_navigation_rotate_left_button_clicked();
  void cool_navigation_rotate_right_button_clicked();
  void cool_navigation_rotate_spin_button_changed();
  void cool_main_entry_changed();
  void cool_main_entry_button_clicked();
  void print_to_cool_main_entry_text_view_output(std::stringstream &out);
  void apply_transform(const std::string &drawable_name,
                       const cool_gl::Matrix &transform);
  void apply_transform_on_mass_centre(const std::string &drawable_name,
                                      const cool_gl::Matrix &transform);
  int run();
  };

} // namespace cool_app
