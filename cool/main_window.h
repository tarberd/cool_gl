#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>


class MainWindow : public Gtk::Window
{
  // Constructor and destructor
public:
  MainWindow();
  virtual             ~MainWindow();

protected:
  //Signal handlers (run when the button are clicked)
  void                  on_button_1();
  void                  on_button_2();

  //Member widgets:
  Gtk::Button           button1;
  Gtk::Button           button2;
  Gtk::Button           buttonQuit;
  Gtk::Grid             mainGrid;
};


#endif // MAINWINDOW_H
