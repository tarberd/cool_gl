#include <iostream>
#include <gtkmm.h>
#include "main_window.h"

int main(int argc, char** argv)
{
    auto app = Gtk::Application::create(argc, argv, "Cool.gl");
    //
    // //Load the GtkBuilder file and instantiate its widgets:
    // auto refBuilder = Gtk::Builder::create();
    // try
    // {
    //     std::string glade_path;
    //
    //     auto glade_file = std::string("window_layout.glade");
    //     if(std::getenv("COOL_GLADE_PATH") == nullptr) {
    //         glade_path = glade_file;
    //     } else {
    //         glade_path = std::string(std::getenv("COOL_GLADE_PATH")) + "/" + glade_file;
    //     }
    //
    //     refBuilder->add_from_file(glade_path.c_str());
    // }
    // catch(const Glib::FileError& ex)
    // {
    //     std::cerr << "FileError: " << ex.what() << std::endl;
    //     return 1;
    // }
    // catch(const Glib::MarkupError& ex)
    // {
    //     std::cerr << "MarkupError: " << ex.what() << std::endl;
    //     return 1;
    // }
    // catch(const Gtk::BuilderError& ex)
    // {
    //     std::cerr << "BuilderError: " << ex.what() << std::endl;
    //     return 1;
    // }
    //
    // Gtk::ApplicationWindow* p_window;
    //
    // refBuilder->get_widget("MainWindow", p_window);
    //
    // Gtk::DrawingArea* p_drawing_area;
    //
    // refBuilder->get_widget("Canvas", p_drawing_area);
    //
    // auto myContext = p_drawing_area->get_window()->create_cairo_context();
    // myContext->set_source_rgb(1.0, 0.0, 0.0);
    // myContext->set_line_width(2.0);
    //
    // app->run(*p_window);

    // Initialize gtkmm and create the main window
	//Glib::RefPtr app = Gtk::Application::create(argc, argv, "www.lucidarme.me");
	MainWindow window;


 //    window.set_title("Cool GL");
 //    window.set_size_request(800, 600);

 //      /* Sets the border width of the window. */
 //    window.set_border_width(10);

 //    window.add(m_box1);
 //    window.add(m_box2);

	// // Create the drawing
	// Drawing dwg;

 //    Gtk::Button m_button("Hello World");
	// // Insert the drawing in the window
	// m_box1.add(dwg);
 //    m_box2.add(m_button);

 //    // dwg.show();
 //    // m_button.show();

 //    window.show_all_children();
	// Start main loop
	return app->run(window);
}
