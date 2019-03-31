#include "cool_app.h"
namespace cool_app {

CoolApp::CoolApp(int argc, char **argv) {
  gtk_application = Gtk::Application::create(argc, argv, "Cool.gl");

  gtk_builder = Gtk::Builder::create();

  gtk_builder->add_from_file(gtk_builder_glade_file);

  // Init widgets
  gtk_builder->get_widget(cool_main_application_window_id,
                          cool_main_application_window);
  gtk_builder->get_widget(cool_drawing_area_id, cool_drawing_area);
  gtk_builder->get_widget(cool_main_entry_id, cool_main_entry);
  gtk_builder->get_widget(cool_main_entry_button_id, cool_main_entry_button);
  gtk_builder->get_widget(cool_main_entry_text_view_output_id,
                          cool_main_entry_text_view_output);
  gtk_builder->get_widget(cool_display_file_tree_view_id,
                          cool_display_file_tree_view);
  gtk_builder->get_widget(cool_navigation_zoom_spin_button_id,
                          cool_navigation_zoom_spin_button);
  gtk_builder->get_widget(cool_navigation_zoom_in_button_id,
                          cool_navigation_zoom_in_button);
  gtk_builder->get_widget(cool_navigation_zoom_out_button_id,
                          cool_navigation_zoom_out_button);
  gtk_builder->get_widget(cool_navigation_button_left_id,
                          cool_navigation_button_left);
  gtk_builder->get_widget(cool_navigation_button_down_id,
                          cool_navigation_button_down);
  gtk_builder->get_widget(cool_navigation_button_up_id,
                          cool_navigation_button_up);
  gtk_builder->get_widget(cool_navigation_button_right_id,
                          cool_navigation_button_right);

  // Connect signal
  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_main_entry->signal_changed().connect(
      [this]() { return cool_main_entry_changed(); });
  cool_main_entry_button->signal_clicked().connect(
      [this]() { return cool_main_entry_button_clicked(); });
  cool_navigation_zoom_spin_button->signal_changed().connect(
      [this]() { return cool_navigation_zoom_spin_button_changed(); });
  cool_navigation_zoom_in_button->signal_clicked().connect(
      [this]() { return cool_navigation_zoom_in_button_clicked(); });
  cool_navigation_zoom_out_button->signal_clicked().connect(
      [this]() { return cool_navigation_zoom_out_button_clicked(); });
  cool_navigation_button_left->signal_clicked().connect(
      [this]() { return cool_navigation_button_left_clicked(); });
  cool_navigation_button_down->signal_clicked().connect(
      [this]() { return cool_navigation_button_down_clicked(); });
  cool_navigation_button_up->signal_clicked().connect(
      [this]() { return cool_navigation_button_up_clicked(); });
  cool_navigation_button_right->signal_clicked().connect(
      [this]() { return cool_navigation_button_right_clicked(); });

  // Drawin Area
  cool_drawing_area->show();

  // Spin Button
  cool_navigation_zoom_spin_button->set_digits(3);
  cool_navigation_zoom_spin_button->set_range(1, 100);
  cool_navigation_zoom_spin_button->set_increments(1, 100);
  cool_navigation_zoom_spin_button->set_value(1);

  object_list = Gtk::ListStore::create(my_columns);

  for (const auto &drawable : drawable_vector) {
    auto row = *object_list->append();
    row[my_columns.column_type] = drawable->type();
    row[my_columns.column_name] = drawable->name();
  }

  cool_display_file_tree_view->set_model(object_list);
  cool_display_file_tree_view->append_column("Type", my_columns.column_type);
  cool_display_file_tree_view->append_column("Nome", my_columns.column_name);
}

bool CoolApp::cool_drawing_area_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
  const double width = static_cast<double>(cool_drawing_area->get_width());
  const double height = static_cast<double>(cool_drawing_area->get_height());

  using cool_gl::Matrix;
  using cool_gl::Vec;

  Vec viewport_begin = {0.0, 0.0, 1.0};
  Vec viewport_end = {width, height, 1.0};

  for (const auto &drawable : drawable_vector) {
    drawable->draw(cr, window_begin, window_end, viewport_begin, viewport_end);
  }

  return true;
}

void CoolApp::cool_navigation_button_down_clicked() {
  window_begin.y -= MOVE_FACTOR;
  window_end.y -= MOVE_FACTOR;

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_button_up_clicked() {
  window_begin.y += MOVE_FACTOR;
  window_end.y += MOVE_FACTOR;

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_button_right_clicked() {
  window_begin.x += MOVE_FACTOR;
  window_end.x += MOVE_FACTOR;

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_button_left_clicked() {
  window_begin.x -= MOVE_FACTOR;
  window_end.x -= MOVE_FACTOR;

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_zoom_out_button_clicked() {
  window_begin.x -= zoom_factor;
  window_begin.y -= zoom_factor;

  window_end.x += zoom_factor;
  window_end.y += zoom_factor;

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_zoom_in_button_clicked() {
  window_begin.x += zoom_factor;
  window_begin.y += zoom_factor;

  window_end.x -= zoom_factor;
  window_end.y -= zoom_factor;

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_zoom_spin_button_changed() {
  zoom_factor = cool_navigation_zoom_spin_button->get_value_as_int();
}

void CoolApp::cool_main_entry_changed() {
  auto cstr = cool_main_entry->get_text();
  create_drawable_entrie_string = std::string{cstr};
}

void CoolApp::cool_main_entry_button_clicked() {
  std::stringstream entrie_stream;
  std::stringstream output_stream;
  output_stream << std::skipws;

  entrie_stream << create_drawable_entrie_string;

  std::string command_string;

  entrie_stream >> command_string;

  if (command_string == "help") {
    output_stream << "Available commands:" << std::endl
                  << "\tpoint [name] [x] [y]" << std::endl
                  << "\tline [name] [begin_x] [begin_y] [end_x] [end_y]"
                  << std::endl
                  << "\tpolygon [name] [list [[x] [y]] ...]" << std::endl
                  << "\ttranslate [name] [delta_x] [delta_y] [delta_z]"
                  << std::endl;

  } else if (command_string == "point") {
    std::string name;

    std::string x_string;
    std::string y_string;

    entrie_stream >> name >> x_string >> y_string;

    double x = std::stod(x_string);
    double y = std::stod(y_string);

    output_stream << "Create: " << command_string << std::endl
                  << "\tname: " << name << std::endl;
    output_stream << "\tpoint position: ";
    output_stream << "(" << x << ", " << y << ") ";
    output_stream << std::endl;

    auto position = cool_gl::Vec{x, y, 1.0};
    drawable_vector.emplace_back(new cool_gl::Point{
        std::move(position), cool_gl::Colour{0.0, 0.0, 0.0}, std::move(name)});

    auto row = *object_list->append();
    row[my_columns.column_type] = drawable_vector.back()->type();
    row[my_columns.column_name] = drawable_vector.back()->name();
  } else if (command_string == "line") {
    std::string name;

    std::string x_begin_string;
    std::string y_begin_string;

    std::string x_end_string;
    std::string y_end_string;

    entrie_stream >> name >> x_begin_string >> y_begin_string >> x_end_string >>
        y_end_string;

    double x_begin = std::stod(x_begin_string);
    double y_begin = std::stod(y_begin_string);
    double x_end = std::stod(x_end_string);
    double y_end = std::stod(y_end_string);

    output_stream << "Create: " << command_string << std::endl
                  << "\tname: " << name << std::endl;
    output_stream << "\tpoint begin and end: ";
    output_stream << "(" << x_begin << ", " << y_begin << ") ";
    output_stream << "(" << x_end << ", " << y_end << ") ";
    output_stream << std::endl;

    auto begin = cool_gl::Vec{x_begin, y_begin, 1.0};
    auto end = cool_gl::Vec{x_end, y_end, 1.0};

    drawable_vector.emplace_back(
        new cool_gl::Line{std::move(begin), std::move(end),
                          cool_gl::Colour{0.0, 0.0, 0.0}, std::move(name)});

    auto row = *object_list->append();
    row[my_columns.column_type] = drawable_vector.back()->type();
    row[my_columns.column_name] = drawable_vector.back()->name();
  } else if (command_string == "polygon") {
    std::string name;

    entrie_stream >> name;

    output_stream << "Create: " << command_string << std::endl
                  << "\tname: " << name << std::endl;
    output_stream << "\tpoint list: ";

    std::string x_string;
    std::string y_string;

    std::vector<cool_gl::Vec> points;

    while (entrie_stream >> x_string >> y_string) {
      double x = std::stod(x_string);
      double y = std::stod(y_string);
      output_stream << "(" << x << ", " << y << ") ";

      points.emplace_back(x, y, 1.0);
    }

    output_stream << std::endl;

    drawable_vector.emplace_back(new cool_gl::Polygon{
        std::move(points), cool_gl::Colour{0.0, 0.0, 0.0}, std::move(name)});

    auto row = *object_list->append();
    row[my_columns.column_type] = drawable_vector.back()->type();
    row[my_columns.column_name] = drawable_vector.back()->name();
  } else if (command_string == "translate") {
    std::string name;

    std::string dx_string;
    std::string dy_string;
    std::string dz_string;

    entrie_stream >> name >> dx_string >> dy_string >> dz_string;
  } else {
    output_stream
        << "Please enter a valid command! Enter help to see valid commands."
        << std::endl;
  }

  std::string command_output_string = output_stream.str();

  auto buffer_end = cool_main_entry_text_view_output->get_buffer()->end();

  buffer_end = cool_main_entry_text_view_output->get_buffer()->insert(
      buffer_end, command_output_string);

  auto end_mark =
      cool_main_entry_text_view_output->get_buffer()->create_mark(buffer_end);

  cool_main_entry_text_view_output->scroll_to(end_mark);

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

int CoolApp::run() {
  return gtk_application->run(*cool_main_application_window);
}

} // namespace cool_app
