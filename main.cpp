#include <iostream>
#include <gtkmm.h>

int main(int argc, char** argv)
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

    Gtk::Window window;

    return app->run(window);
}
