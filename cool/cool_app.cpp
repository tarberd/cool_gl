#include "cool_app.h"
namespace cool_app {

CoolApp::CoolApp() {
  gtk_application = Gtk::Application::create("Cool.gl");

  gtk_builder = Gtk::Builder::create();

  gtk_builder->add_from_file(gtk_builder_glade_file);

  // Init widgets
  gtk_builder->get_widget(cool_main_application_window_id,
                          cool_main_application_window);
  gtk_builder->get_widget(cool_drawing_area_id, cool_drawing_area);
  gtk_builder->get_widget(cool_main_entry_id, cool_main_entry);
  gtk_builder->get_widget(cool_main_entry_button_id, cool_main_entry_button);
  gtk_builder->get_widget(cool_display_file_tree_view_id,
                          cool_display_file_tree_view);
  gtk_builder->get_widget(cool_navigation_zoom_spin_button_id,
                          cool_navigation_zoom_spin_button);
  gtk_builder->get_widget(cool_navigation_button_left_id,
                          cool_navigation_button_left);
  gtk_builder->get_widget(cool_navigation_button_down_id,
                          cool_navigation_button_down);
  gtk_builder->get_widget(cool_navigation_button_up_id,
                          cool_navigation_button_up);
  gtk_builder->get_widget(cool_navigation_button_right_id,
                          cool_navigation_button_right);

  // set_digits int
  //     range int int
  //     increments int int
  //     value int

  // Connect signal
  cool_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });

  cool_main_entry->signal_changed().connect(
      [this]() { return create_drawable_entry_value(); });

  cool_main_entry_button->signal_clicked().connect(
      [this]() { return create_drawable_from_entry(); });

  cool_navigation_zoom_spin_button->signal_changed().connect([this]() { return zoom_value(); });

  cool_navigation_button_left->signal_clicked().connect([this]() { return move_left(); });
  cool_navigation_button_down->signal_clicked().connect([this]() { return move_down(); });
  cool_navigation_button_up->signal_clicked().connect([this]() { return move_up(); });
  cool_navigation_button_right->signal_clicked().connect([this]() { return move_right(); });

  cool_drawing_area->show();

  object_list = Gtk::ListStore::create(my_columns);

  for (const auto &drawable : drawable_vector) {
    auto row = *object_list->append();
    row[my_columns.column_type] = drawable->type();
    row[my_columns.column_name] = drawable->name();
  }

  cool_display_file_tree_view->set_model(object_list);
  std::cout << "OIE" << std::endl;
  cool_display_file_tree_view->append_column("Type", my_columns.column_type);
  cool_display_file_tree_view->append_column("Nome", my_columns.column_name);
}

bool CoolApp::draw_callback(const Cairo::RefPtr<Cairo::Context> &cr) {
  const double width = static_cast<double>(cool_drawing_area->get_width());
  const double height = static_cast<double>(cool_drawing_area->get_height());

  using cool_gl::Vec;

  Vec viewport_begin = {0.0, 0.0, 1.0};
  Vec viewport_end = {width, height, 1.0};

  for (const auto &drawable : drawable_vector) {
    drawable->draw(cr, window_begin, window_end, viewport_begin, viewport_end);
  }

  return true;
}

void CoolApp::move_down() {
  window_begin.y -= MOVE_FACTOR;
  window_end.y -= MOVE_FACTOR;

  cool_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  cool_drawing_area->queue_draw();
}

void CoolApp::move_up() {
  window_begin.y += MOVE_FACTOR;
  window_end.y += MOVE_FACTOR;

  cool_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  cool_drawing_area->queue_draw();
}

void CoolApp::move_right() {
  window_begin.x += MOVE_FACTOR;
  window_end.x += MOVE_FACTOR;

  cool_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  cool_drawing_area->queue_draw();
}

void CoolApp::move_left() {
  window_begin.x -= MOVE_FACTOR;
  window_end.x -= MOVE_FACTOR;

  cool_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  cool_drawing_area->queue_draw();
}

void CoolApp::out_callback() {
  window_begin.x -= zoom_factor;
  window_begin.y -= zoom_factor;

  window_end.x += zoom_factor;
  window_end.y += zoom_factor;

  cool_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  cool_drawing_area->queue_draw();
}

void CoolApp::in_callback() {
  window_begin.x += zoom_factor;
  window_begin.y += zoom_factor;

  window_end.x -= zoom_factor;
  window_end.y -= zoom_factor;

  cool_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  cool_drawing_area->queue_draw();
}

void CoolApp::zoom_value() {
  const char *c = cool_navigation_zoom_spin_button->get_text().c_str();

  zoom_factor = atol(c);
}

void CoolApp::create_drawable_entry_value() {
  const char *cstr = cool_main_entry->get_text().c_str();

  create_drawable_entrie_string = std::string{cstr};
}

void CoolApp::create_drawable_from_entry() {
  std::cout << create_drawable_entrie_string << std::endl;

  std::stringstream entrie_stream;
  entrie_stream << create_drawable_entrie_string;

  std::string current_string;

  entrie_stream >> current_string;

  if (current_string == "point") {
    std::string name;
    std::string x_string;
    std::string y_string;

    entrie_stream >> name >> x_string >> y_string;

    auto position = cool_gl::Vec{std::stod(x_string), std::stod(y_string), 1.0};
    drawable_vector.emplace_back(
        new cool_gl::Point{position, cool_gl::Colour{0.0, 0.0, 0.0}, name});

    auto row = *object_list->append();
    row[my_columns.column_type] = drawable_vector.back()->type();
    row[my_columns.column_name] = drawable_vector.back()->name();
  }
  if (current_string == "line") {
    std::string name;

    std::string x_begin_string;
    std::string y_begin_string;

    std::string x_end_string;
    std::string y_end_string;

    entrie_stream >> name >> x_begin_string >> y_begin_string >> x_end_string >>
        y_end_string;

    auto begin =
        cool_gl::Vec{std::stod(x_begin_string), std::stod(y_begin_string), 1.0};
    auto end =
        cool_gl::Vec{std::stod(x_end_string), std::stod(y_end_string), 1.0};

    drawable_vector.emplace_back(
        new cool_gl::Line{begin, end, cool_gl::Colour{0.0, 0.0, 0.0}, name});

    auto row = *object_list->append();
    row[my_columns.column_type] = drawable_vector.back()->type();
    row[my_columns.column_name] = drawable_vector.back()->name();
  }
  if (current_string == "polygon") {
    std::string name;

    std::vector<cool_gl::Vec> points;

    std::string x_string;
    std::string y_string;

    entrie_stream >> name;
    while (entrie_stream >> x_string >> y_string) {

      points.emplace_back(std::stod(x_string), std::stod(y_string), 1.0);
    }

    drawable_vector.emplace_back(new cool_gl::Polygon{
        std::move(points), cool_gl::Colour{0.0, 0.0, 0.0}, name});

    auto row = *object_list->append();
    row[my_columns.column_type] = drawable_vector.back()->type();
    row[my_columns.column_name] = drawable_vector.back()->name();
  }

  cool_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  cool_drawing_area->queue_draw();
}

int CoolApp::run(int argc, char **argv) {
  return gtk_application->run(*cool_main_application_window);
}
} // namespace cool_app
