#include <cool_gl/cool_gl.h>
#include <gtkmm.h>
#include <iostream>

const double ZOOM_FACTOR = 0.8;
const double MOVE_FACTOR = 1;

double zoom = 0.001;
double tx = 1.0;
double ty = 1.0;

Gtk::Window *glade_window;

Gtk::DrawingArea *glade_drawing_area;


bool draw_callback(const Cairo::RefPtr<Cairo::Context> &cr) {
  const int width = 600;
  const int height = 800;
  glade_drawing_area->set_size_request(800, 600);

  auto line = cool_gl::Line{
      cool_gl::Point{1.0, 1.0},
      cool_gl::Point{static_cast<double>(width), static_cast<double>(height)},
      cool_gl::Colour{0.8, 0.0, 0.0}};

  cr->scale(width*zoom, height*zoom);
  cr->translate(tx, ty);
  line.draw(cr);

  return true;
}

void move_down(){
  ty += MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void move_up(){
  ty -= MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void move_right(){
  tx += MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void move_left(){
  tx -= 1;

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void out_callback() {
  zoom *= ZOOM_FACTOR;
  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

void in_callback() {
  zoom /= ZOOM_FACTOR;
  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));
  glade_drawing_area->queue_draw();
}

int main(int argc, char **argv) {
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

  Gtk::Button *zoom_out;
  Gtk::Button *zoom_in;
  Gtk::Button *down;
  Gtk::Button *up;
  Gtk::Button *left;
  Gtk::Button *right;

  builder->get_widget("zoom_out_button", zoom_out);
  builder->get_widget("zoom_in_button", zoom_in);
  builder->get_widget("down", down);
  builder->get_widget("up", up);
  builder->get_widget("right", right);
  builder->get_widget("left", left);


  if (glade_window == nullptr) {
    throw std::runtime_error(
        "builder could not find: glade_button_1_id widget");
  }

  zoom_out->signal_clicked().connect(sigc::ptr_fun(out_callback));
  zoom_in->signal_clicked().connect(sigc::ptr_fun(in_callback));
  down->signal_clicked().connect(sigc::ptr_fun(move_down));
  right->signal_clicked().connect(sigc::ptr_fun(move_right));
  left->signal_clicked().connect(sigc::ptr_fun(move_left));
  up->signal_clicked().connect(sigc::ptr_fun(move_up));

  builder->get_widget("cool_main_gtk_drawing_area_id", glade_drawing_area);

  if (glade_drawing_area == nullptr) {
    throw std::runtime_error(
        "builder could not find: cool_main_gtk_drawing_area_id widget");
  }

  glade_drawing_area->signal_draw().connect(sigc::ptr_fun(draw_callback));

  glade_drawing_area->show();

  auto result = app->run(*glade_window);

  return result;
}
