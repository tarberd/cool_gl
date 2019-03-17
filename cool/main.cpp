#include <cool_gl/cool_gl.h>
#include <gtkmm.h>
#include <iostream>

class Drawing : public Gtk::DrawingArea {
protected:
  // Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) final {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    auto line = cool_gl::Line{
        cool_gl::Point{0.0, 0.0},
        cool_gl::Point{static_cast<double>(width), static_cast<double>(height)},
        cool_gl::Colour{0.8, 0.0, 0.0}};

    line.draw(cr);

    return true;
  }
};

int main(int argc, char **argv) {
  auto app = Gtk::Application::create(argc, argv, "Cool.gl");

  Gtk::Window window;

  // Create the drawing
  Drawing dwg;

  // Insert the drawing in the window
  window.add(dwg);

  // Show the drawing
  dwg.show();

  // Start main loop
  return app->run(window);
}
