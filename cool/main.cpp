#include <cool_gl/cool_gl.h>
#include <gtkmm.h>
#include <iostream>

bool draw_callback(const Cairo::RefPtr<Cairo::Context> &cr) {
  const int width = 600;
  const int height = 800;

  auto line = cool_gl::Line{
      cool_gl::Point{0.0, 0.0},
      cool_gl::Point{static_cast<double>(width), static_cast<double>(height)},
      cool_gl::Colour{0.8, 0.0, 0.0}};

  line.draw(cr);

  return true;
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

  Gtk::Window *glade_window;

  builder->get_widget("cool_main_gtk_window_id", glade_window);

  if (glade_window == nullptr) {
    throw std::runtime_error(
        "builder could not find: cool_main_gtk_window widget");
  }

  Gtk::DrawingArea *glade_drawing_area;

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
