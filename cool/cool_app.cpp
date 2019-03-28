#include "cool_app.h"
namespace cool_app {
bool CoolApp::draw_callback(const Cairo::RefPtr<Cairo::Context> &cr) {
  const int width = glade_drawing_area->get_width();
  const int height = glade_drawing_area->get_height();

  using cool_gl::Vec3;

  Vec3 viewport_begin = {0.0, 0.0, 1.0};
  Vec3 viewport_end = {static_cast<double>(width), static_cast<double>(height),
                       1};

  for (const auto &drawable : drawable_vector) {
    drawable->draw(cr, window_begin, window_end, viewport_begin, viewport_end);
  }

  return true;
}

void CoolApp::move_down() {
  window_begin.y -= MOVE_FACTOR;
  window_end.y -= MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  glade_drawing_area->queue_draw();
}

void CoolApp::move_up() {
  window_begin.y += MOVE_FACTOR;
  window_end.y += MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  glade_drawing_area->queue_draw();
}

void CoolApp::move_right() {
  window_begin.x += MOVE_FACTOR;
  window_end.x += MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  glade_drawing_area->queue_draw();
}

void CoolApp::move_left() {
  window_begin.x -= MOVE_FACTOR;
  window_end.x -= MOVE_FACTOR;

  glade_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  glade_drawing_area->queue_draw();
}

void CoolApp::out_callback() {
  window_begin.x -= zoom_factor;
  window_begin.y -= zoom_factor;

  window_end.x += zoom_factor;
  window_end.y += zoom_factor;

  glade_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  glade_drawing_area->queue_draw();
}

void CoolApp::in_callback() {
  window_begin.x += zoom_factor;
  window_begin.y += zoom_factor;

  window_end.x -= zoom_factor;
  window_end.y -= zoom_factor;

  glade_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  glade_drawing_area->queue_draw();
}

void CoolApp::zoom_value() {
  const char *c = zoom->get_text().c_str();

  zoom_factor = atol(c);
}

void CoolApp::create_drawable_entry_value() {
  const char *cstr = add_drawable_comand_entry->get_text().c_str();

  create_drawable_entrie_string = std::string{cstr};
}

void CoolApp::show_add_drawable_dialog() { dialog_add_drawable->show(); }

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

    auto position =
        cool_gl::Vec3{std::stod(x_string), std::stod(y_string), 1.0};
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

    auto begin = cool_gl::Vec3{std::stod(x_begin_string),
                               std::stod(y_begin_string), 1.0};
    auto end =
        cool_gl::Vec3{std::stod(x_end_string), std::stod(y_end_string), 1.0};

    drawable_vector.emplace_back(
        new cool_gl::Line{begin, end, cool_gl::Colour{0.0, 0.0, 0.0}, name});

    auto row = *object_list->append();
    row[my_columns.column_type] = drawable_vector.back()->type();
    row[my_columns.column_name] = drawable_vector.back()->name();
  }
  if (current_string == "polygon") {
    std::string name;

    std::vector<cool_gl::Vec3> points;

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

  glade_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });
  glade_drawing_area->queue_draw();
}

int CoolApp::run(int argc, char **argv) {
  drawable_vector.emplace_back(new cool_gl::Line{
      cool_gl::Vec3{0.0, 0.0, 1.0}, cool_gl::Vec3{40.0, 40.0, 1.0},
      cool_gl::Colour{0.0, 0.0, 0.0}, "line 1"});

  drawable_vector.emplace_back(new cool_gl::Line{
      cool_gl::Vec3{40.0, 0.0, 1.0}, cool_gl::Vec3{0.0, 40.0, 1.0},
      cool_gl::Colour{0.8, 0.0, 0.0}, "line 2"});

  drawable_vector.emplace_back(
      new cool_gl::Point{cool_gl::Vec3{12.0, 20.0, 1.0},
                         cool_gl::Colour{0.2, 0.7, 0.4}, "banana"});

  using cool_gl::Vec3;

  drawable_vector.emplace_back(new cool_gl::Polygon{
      std::vector<Vec3>{Vec3{2.0, 2.0, 1.0}, Vec3{2.0, 38.0, 1.0},
                        Vec3{38.0, 38.0, 1.0}, Vec3{38.0, 2.0, 1.0}},
      cool_gl::Colour{0.7, 0.2, 0.4}, "coisa feia"});

  auto app = Gtk::Application::create(argc, argv, "Cool.gl");

  auto builder = Gtk::Builder::create();

  builder->add_from_file("cool_glade.glade");

  builder->get_widget("cool_main_gtk_window_id", glade_window);

  if (glade_window == nullptr) {
    throw std::runtime_error(
        "builder could not find: cool_main_gtk_window_id widget");
  }

  glade_window->set_default_size(800, 600);

  builder->get_widget("cool_add_drawable_dialog_id", dialog_add_drawable);

  Gtk::Button *zoom_out;
  Gtk::Button *zoom_in;
  Gtk::Button *down;
  Gtk::Button *up;
  Gtk::Button *left;
  Gtk::Button *right;
  Gtk::Button *button_dialog_add_drawable;
  Gtk::Button *button_dialog_create_drawable;

  builder->get_widget("zoom_out_button", zoom_out);
  builder->get_widget("zoom_in_button", zoom_in);
  builder->get_widget("down", down);
  builder->get_widget("up", up);
  builder->get_widget("right", right);
  builder->get_widget("left", left);

  builder->get_widget("zoom_factor_input", zoom);
  builder->get_widget("tree_view_id", object_tree);

  builder->get_widget("add_button_id", button_dialog_add_drawable);
  builder->get_widget("button_dialog_create_id", button_dialog_create_drawable);

  button_dialog_add_drawable->signal_clicked().connect(
      [this]() { return show_add_drawable_dialog(); });

  button_dialog_create_drawable->signal_clicked().connect(
      [this]() { return create_drawable_from_entry(); });

  builder->get_widget("entry_add_drawable_dialog_id",
                      add_drawable_comand_entry);

  add_drawable_comand_entry->get_buffer()->set_max_length(50);
  add_drawable_comand_entry->signal_changed().connect(
      [this]() { return create_drawable_entry_value(); });

  zoom_out->signal_clicked().connect([this]() { return out_callback(); });
  zoom_in->signal_clicked().connect([this]() { return in_callback(); });
  down->signal_clicked().connect([this]() { return move_down(); });
  right->signal_clicked().connect([this]() { return move_right(); });
  left->signal_clicked().connect([this]() { return move_left(); });
  up->signal_clicked().connect([this]() { return move_up(); });

  zoom->signal_changed().connect([this]() { return zoom_value(); });
  zoom->set_text("1");

  builder->get_widget("cool_main_gtk_drawing_area_id", glade_drawing_area);

  if (glade_drawing_area == nullptr) {
    throw std::runtime_error(
        "builder could not find: cool_main_gtk_drawing_area_id widget");
  }

  object_list = Gtk::ListStore::create(my_columns);

  for (const auto &drawable : drawable_vector) {
    auto row = *object_list->append();
    row[my_columns.column_type] = drawable->type();
    row[my_columns.column_name] = drawable->name();
  }

  object_tree->set_model(object_list);
  object_tree->append_column("Type", my_columns.column_type);
  object_tree->append_column("Nome", my_columns.column_name);

  glade_drawing_area->set_size_request(600, 600);

  glade_drawing_area->signal_draw().connect(
      [this](const Cairo::RefPtr<Cairo::Context> &cr) {
        return draw_callback(cr);
      });

  glade_drawing_area->show();

  return app->run(*glade_window);
}
} // namespace cool_app
