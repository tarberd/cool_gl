#include "model_columns.h"
#include <cool_gl/cool_gl.h>
#include <gtkmm.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

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

bool draw_callback(const Cairo::RefPtr<Cairo::Context> &cr) {
  const int width = glade_drawing_area->get_width();
  const int height = glade_drawing_area->get_height();

  using cool_gl::Vec3;

  Vec3 viewport_begin = {0.0, 0.0, 1.0};
  Vec3 viewport_end = {static_cast<double>(width), static_cast<double>(height),
                     1};

  for(const auto & drawable : drawable_vector){
      drawable->draw(cr, window_begin, window_end, viewport_begin, viewport_end);
  }

  return true;
}

void move_down(){
  window_begin.y -= MOVE_FACTOR;
  window_end.y -= MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void move_up(){
  window_begin.y += MOVE_FACTOR;
  window_end.y += MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void move_right(){
  window_begin.x += MOVE_FACTOR;
  window_end.x += MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void move_left(){
  window_begin.x -= MOVE_FACTOR;
  window_end.x -= MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void out_callback() {
  window_begin.x -= zoom_factor;
  window_begin.y -= zoom_factor;

  window_end.x += zoom_factor;
  window_end.y += zoom_factor;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void in_callback() {
  window_begin.x += zoom_factor;
  window_begin.y += zoom_factor;

  window_end.x -= zoom_factor;
  window_end.y -= zoom_factor;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void zoom_value() {
  const char *c = zoom->get_text().c_str();

  zoom_factor = atol(c);
}

void create_drawable_entry_value() {
  const char *cstr = add_drawable_comand_entry->get_text().c_str();

  create_drawable_entrie_string = std::string{cstr};
}

void show_add_drawable_dialog() { dialog_add_drawable->show(); }

void create_drawable_from_entry() {
  std::cout << create_drawable_entrie_string << std::endl;

  std::stringstream entrie_stream;
  entrie_stream << create_drawable_entrie_string;

  std::string current_string;

  entrie_stream >> current_string;

  std::cout << current_string << std::endl;

  if (current_string == "point") {
    std::string name;
    std::string x_string;
    std::string y_string;

    entrie_stream >> name;
    entrie_stream >> x_string;
    entrie_stream >> y_string;

    std::cout << name << x_string << y_string << std::endl;

    auto position =
        cool_gl::Vec3{std::stod(x_string), std::stod(y_string), 1.0};
    drawable_vector.emplace_back(
        new cool_gl::Point{position, cool_gl::Colour{0.0, 0.0, 0.0}, name});
  }

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

int main(int argc, char **argv) {
  drawable_vector.emplace_back(new cool_gl::Line{
      cool_gl::Vec3{0.0, 0.0, 1.0}, cool_gl::Vec3{40.0, 40.0, 1.0},
      cool_gl::Colour{0.0, 0.0, 0.0}, "line 1"});

  drawable_vector.emplace_back(new cool_gl::Line{
      cool_gl::Vec3{40.0, 0.0, 1.0}, cool_gl::Vec3{0.0, 40.0, 1.0},
      cool_gl::Colour{0.8, 0.0, 0.0}, "line 2"});

  drawable_vector.emplace_back(
      new cool_gl::Point{cool_gl::Vec3{12.0, 20.0, 1.0},
                         cool_gl::Colour{0.2, 0.7, 0.4}, "banana"});

  using cool_gl::Vec3;

  drawable_vector.emplace_back(new cool_gl::Polygon{
      std::vector<Vec3>{Vec3{2.0, 2.0, 1.0}, Vec3{2.0, 38.0, 1.0},
                        Vec3{38.0, 38.0, 1.0}, Vec3{38.0, 2.0, 1.0}},
      cool_gl::Colour{0.7, 0.2, 0.4}, "coisa feia"});


  auto app = Gtk::Application::create(argc, argv, "Cool.gl");

  auto builder = Gtk::Builder::create();

  try {
    builder->add_from_file("cool_glade.glade");
  } catch (const Glib::FileError &ex) {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  } catch (const Glib::MarkupError &ex) {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  } catch (const Gtk::BuilderError &ex) {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  builder->get_widget("cool_main_gtk_window_id", glade_window);

  if (glade_window == nullptr) {
    throw std::runtime_error(
        "builder could not find: cool_main_gtk_window_id widget");
  }

  glade_window->set_default_size(800, 600);

  builder->get_widget("cool_add_drawable_dialog_id", dialog_add_drawable);

  Gtk::Button *zoom_out;
  Gtk::Button *zoom_in;
  Gtk::Button *down;
  Gtk::Button *up;
  Gtk::Button *left;
  Gtk::Button *right;
  Gtk::Button *button_dialog_add_drawable;
  Gtk::Button *button_dialog_create_drawable;

  builder->get_widget("zoom_out_button", zoom_out);
  builder->get_widget("zoom_in_button", zoom_in);
  builder->get_widget("down", down);
  builder->get_widget("up", up);
  builder->get_widget("right", right);
  builder->get_widget("left", left);

  builder->get_widget("zoom_factor_input", zoom);
  builder->get_widget("tree_view_id", object_tree);

  builder->get_widget("add_button_id", button_dialog_add_drawable);
  builder->get_widget("button_dialog_create_id", button_dialog_create_drawable);

  button_dialog_add_drawable->signal_clicked().connect(
      sigc::ptr_fun(show_add_drawable_dialog));

  button_dialog_create_drawable->signal_clicked().connect(
      sigc::ptr_fun(create_drawable_from_entry));

  builder->get_widget("entry_add_drawable_dialog_id",
                      add_drawable_comand_entry);

  add_drawable_comand_entry->set_max_length(150);
  add_drawable_comand_entry->signal_changed().connect(
      sigc::ptr_fun(create_drawable_entry_value));

  zoom_out->signal_clicked().connect(sigc::ptr_fun(out_callback));
  zoom_in->signal_clicked().connect(sigc::ptr_fun(in_callback));
  down->signal_clicked().connect(sigc::ptr_fun(move_down));
  right->signal_clicked().connect(sigc::ptr_fun(move_right));
  left->signal_clicked().connect(sigc::ptr_fun(move_left));
  up->signal_clicked().connect(sigc::ptr_fun(move_up));

  zoom->signal_changed().connect(sigc::ptr_fun(zoom_value));
  zoom->set_text("1");

  builder->get_widget("cool_main_gtk_drawing_area_id", glade_drawing_area);

  if (glade_drawing_area == nullptr) {
    throw std::runtime_error(
        "builder could not find: cool_main_gtk_drawing_area_id widget");
  }

  object_list = Gtk::ListStore::create(my_columns);

  for(const auto & drawable : drawable_vector) {
    auto row = *object_list->append();
    row[my_columns.column_type] = drawable->type();
    row[my_columns.column_name] = drawable->name();
  }

  object_tree->set_model(object_list);
  object_tree->append_column("Type", my_columns.column_type);
  object_tree->append_column("Nome", my_columns.column_name);

  glade_drawing_area->set_size_request(600, 600);

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));

  glade_drawing_area->show();

  auto result = app->run(*glade_window);

  return result;
}
