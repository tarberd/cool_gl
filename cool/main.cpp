#include <iostream>
#include <gtkmm.h>

class Drawing : public Gtk::DrawingArea
{
protected:
	//Override default signal handler:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
    {
        // Draw the first circle
        cr->arc(75,75,50,0,2*M_PI);
        cr->set_source_rgba(0, 0, 1.0,0.5);
        cr->fill();
        cr->stroke();

        cr->set_source_rgba(0, 1, 0,0.5);
        cr->arc(100,125,50,0,2*M_PI);
        cr->fill();
        cr->stroke();

        cr->set_source_rgba(1, 0, 0,0.5);
        cr->arc(125,75,50,0,2*M_PI);
        cr->fill();
        cr->stroke();

        return true;
    }
};

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
