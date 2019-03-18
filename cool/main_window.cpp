#include "main_window.h"
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

// Constructor of the main Window (build ui interface).
MainWindow::MainWindow()
{
  // Initialize the main window
  this->set_title("CoolGL");
  this->set_border_width(10);
  this->set_size_request(800, 600);

  // Create and connect the first button
  button1.add_label("Button 1");
  button1.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_button_1));
  mainGrid.attach(button1,0,0,1,1);
  mainGrid.attach(button2,1,0,1,1);
  mainGrid.attach(buttonQuit,0,1,2,1);

  // Create and connect the second button
  button2.add_label("Button 2");
  button2.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_button_2));

  // Create and connect the Quit button
  buttonQuit.add_label("Quit");
  buttonQuit.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::close));

  //Create Drawing
  Drawing dwg;

  // mainGrid.attach(dwg,100,20,3,2);

  // Display the main grid in the main window
  mainGrid.show_all();
  dwg.show();
  // add(dwg);
  add(mainGrid);
}


// Destructor of the class
MainWindow::~MainWindow()
{}


// Call when the first button is clicked
void MainWindow::on_button_1()
{
  std::cout << "Button 1" << std::endl;
}


// Call when the second button is clicked
void MainWindow::on_button_2()
{
  std::cout << "Button 2" << std::endl;
}
