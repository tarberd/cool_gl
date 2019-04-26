#include "cool_app.h"
namespace cool_app {

void print_help(std::stringstream &out) {
  out << "Available commands:" << std::endl
      << "\tpoint [object] [x] [y]" << std::endl
      << "\tline [object] [begin_x] [begin_y] [end_x] [end_y]" << std::endl
      << "\tpolygon [object] [list [[x] [y]] ...]" << std::endl
      << "\ttranslate [object] [delta_x] [delta_y] [delta_z]" << std::endl
      << "\tscale [object] [scale_x] [scale_y] [scale_z]" << std::endl
      << "\trotate [object] [rad]" << std::endl
      << "\tclip_line_cohen_sutherland" << std::endl
      << "\tclip_line_liang_barsky" << std::endl;
}

void CoolApp::print_to_cool_main_entry_text_view_output(
    std::stringstream &out) {
  std::string command_output_string = out.str();

  auto buffer_end = cool_main_entry_text_view_output->get_buffer()->end();

  buffer_end = cool_main_entry_text_view_output->get_buffer()->insert(
      buffer_end, command_output_string);

  auto end_mark =
      cool_main_entry_text_view_output->get_buffer()->create_mark(buffer_end);

  cool_main_entry_text_view_output->scroll_to(end_mark);
}

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
  gtk_builder->get_widget(cool_navigation_rotate_spin_button_id,
                          cool_navigation_rotate_spin_button);
  gtk_builder->get_widget(cool_navigation_rotate_left_button_id,
                          cool_navigation_rotate_left_button);
  gtk_builder->get_widget(cool_navigation_rotate_right_button_id,
                          cool_navigation_rotate_right_button);

  // Connect signal
  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_main_entry->signal_changed().connect(
      [this]() { return cool_main_entry_changed(); });
  cool_main_entry_button->signal_clicked().connect(
      [this]() { return cool_main_entry_button_clicked(); });
  cool_main_entry->signal_activate().connect(
    [this]() { return cool_main_entry_button_clicked(); });
  cool_navigation_zoom_spin_button->signal_changed().connect(
      [this]() { return cool_navigation_zoom_spin_button_changed(); });
  cool_navigation_zoom_in_button->signal_clicked().connect(
      [this]() { return cool_navigation_zoom_in_button_clicked(); });
  cool_navigation_zoom_out_button->signal_clicked().connect(
      [this]() { return cool_navigation_zoom_out_button_clicked(); });
  cool_navigation_button_left->signal_clicked().connect(
      [this]() { return cool_navigation_move_left_button_clicked(); });
  cool_navigation_button_down->signal_clicked().connect(
      [this]() { return cool_navigation_move_down_button_clicked(); });
  cool_navigation_button_up->signal_clicked().connect(
      [this]() { return cool_navigation_move_up_button_clicked(); });
  cool_navigation_button_right->signal_clicked().connect(
      [this]() { return cool_navigation_move_right_button_clicked(); });
  cool_navigation_rotate_spin_button->signal_changed().connect(
      [this]() { return cool_navigation_rotate_spin_button_changed(); });
  cool_navigation_rotate_left_button->signal_clicked().connect(
      [this]() { return cool_navigation_rotate_left_button_clicked(); });
  cool_navigation_rotate_right_button->signal_clicked().connect(
      [this]() { return cool_navigation_rotate_right_button_clicked(); });

  // Drawin Area
  cool_drawing_area->show();

  // Zoom Spin Button
  cool_navigation_zoom_spin_button->set_digits(3);
  cool_navigation_zoom_spin_button->set_range(1, 100);
  cool_navigation_zoom_spin_button->set_increments(1, 100);
  cool_navigation_zoom_spin_button->set_value(1);

  // Rotate Spin Button
  cool_navigation_rotate_spin_button->set_digits(3);
  cool_navigation_rotate_spin_button->set_range(0, 4);
  cool_navigation_rotate_spin_button->set_increments(0.15, 1.0);
  cool_navigation_rotate_spin_button->set_value(0.15);

  // Print Help to text view
  std::stringstream out;
  print_help(out);
  print_to_cool_main_entry_text_view_output(out);

  object_list = Gtk::ListStore::create(my_columns);

  display_file.emplace_back(
      new cool_gl::Line{cool_gl::Vec{0.0, -100.0}, cool_gl::Vec{0.0, 100.0},
                        cool_gl::Colour{1.0, 0.0, 0.0}, "x_line"});
  display_file.emplace_back(
      new cool_gl::Line{cool_gl::Vec{100.0, 0.0}, cool_gl::Vec{-100.0, 0.0},
                        cool_gl::Colour{1.0, 0.0, 0.0}, "x_line"});

  for (const auto &drawable : display_file) {
    auto row = *object_list->append();
    row[my_columns.column_type] = drawable->type();
    row[my_columns.column_name] = drawable->name();
  }

  cool_display_file_tree_view->set_model(object_list);
  cool_display_file_tree_view->append_column("Type", my_columns.column_type);
  cool_display_file_tree_view->append_column("Nome", my_columns.column_name);

  // Create debbuging object
#ifdef NDEBUG
#else
  cool_main_entry->set_text("polygon test_square 10 10 10 20 20 20 20 10");
  cool_main_entry_changed();
  cool_main_entry_button_clicked();
  cool_main_entry->set_text("polygon test_triangle -10 -10 -10 -20 -20 -20");
  cool_main_entry_changed();
  cool_main_entry_button_clicked();
#endif
}

bool CoolApp::cool_drawing_area_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
  const double width = static_cast<double>(cool_drawing_area->get_width());
  const double height = static_cast<double>(cool_drawing_area->get_height());

  using cool_gl::Matrix;
  using cool_gl::Vec;

  Vec viewport_begin = {0.0, 0.0};
  Vec viewport_end = {width, height};

  auto normalized_display_file =
      window.create_normalized_display_file(display_file);

  std::vector<Vec> points = { cool_gl::Vec{-1, -1},
                              cool_gl::Vec{-1, 1},
                              cool_gl::Vec{1, 1},
                              cool_gl::Vec{1, -1}
                            };

  auto window_frame =
      cool_gl::Polygon{points, cool_gl::Colour{0.0, 0.0, 1.0}, "window_frame"};

  window_frame.draw(cr, viewport_begin, viewport_end);

  for (const auto &drawable : normalized_display_file) {
    drawable->draw(cr, viewport_begin, viewport_end);
  }

  return true;
}

void CoolApp::cool_navigation_move_down_button_clicked() {
  window.move_up(-1.0 * MOVE_FACTOR);

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_move_up_button_clicked() {
  window.move_up(MOVE_FACTOR);

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_move_right_button_clicked() {
  window.move_right(MOVE_FACTOR);

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_move_left_button_clicked() {
  window.move_right(-1.0 * MOVE_FACTOR);

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_zoom_out_button_clicked() {
  window.height -= zoom_factor;
  window.width -= zoom_factor;

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_zoom_in_button_clicked() {
  window.height += zoom_factor;
  window.width += zoom_factor;

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_rotate_left_button_clicked() {
  auto rotate = cool_gl::create_rotate_transform(-1.0 * rotate_factor);

  window.view_up = cool_gl::multiply(rotate, window.view_up);
  window.view_right = cool_gl::multiply(rotate, window.view_right);

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_rotate_right_button_clicked() {
  auto rotate = cool_gl::create_rotate_transform(rotate_factor);

  window.view_up = cool_gl::multiply(rotate, window.view_up);
  window.view_right = cool_gl::multiply(rotate, window.view_right);

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

void CoolApp::cool_navigation_zoom_spin_button_changed() {
  zoom_factor = cool_navigation_zoom_spin_button->get_value();
}

void CoolApp::cool_navigation_rotate_spin_button_changed() {
  rotate_factor = cool_navigation_rotate_spin_button->get_value();
}

void CoolApp::cool_main_entry_changed() {
  auto cstr = cool_main_entry->get_text();
  create_drawable_entrie_string = std::string{cstr};
}

void CoolApp::apply_transform(const std::string &drawable_name,
                              const cool_gl::Matrix &transform) {
  for (const auto &drawable : display_file) {
    if (drawable->name() == drawable_name) {
      drawable->transform(transform);
    }
  }
}

void CoolApp::apply_transform_on_mass_centre(const std::string &drawable_name,
                                             const cool_gl::Matrix &transform) {
  for (const auto &drawable : display_file) {
    if (drawable->name() == drawable_name) {
      auto mass_centre = drawable->mass_centre();

      auto translate_to_centre = cool_gl::create_translate_transform(
          -1 * mass_centre.x, -1 * mass_centre.y, -1 * mass_centre.z);

      auto translate_from_centre = cool_gl::create_translate_transform(
          mass_centre.x, mass_centre.y, mass_centre.z);

      auto final_transform = cool_gl::multiply(translate_to_centre, transform);
      final_transform =
          cool_gl::multiply(final_transform, translate_from_centre);

      drawable->transform(final_transform);
    }
  }
}

void CoolApp::cool_main_entry_button_clicked() {
  std::stringstream entrie_stream;
  std::stringstream output_stream;
  output_stream << std::skipws;

  entrie_stream << create_drawable_entrie_string;

  std::string command_string;

  entrie_stream >> command_string;

  if (command_string == "help") {
    print_help(output_stream);

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
    display_file.emplace_back(new cool_gl::Point{
        std::move(position), cool_gl::Colour{0.0, 0.0, 0.0}, std::move(name)});

    auto row = *object_list->append();
    row[my_columns.column_type] = display_file.back()->type();
    row[my_columns.column_name] = display_file.back()->name();
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

    display_file.emplace_back(
        new cool_gl::Line{std::move(begin), std::move(end),
                          cool_gl::Colour{0.0, 0.0, 0.0}, std::move(name)});

    auto row = *object_list->append();
    row[my_columns.column_type] = display_file.back()->type();
    row[my_columns.column_name] = display_file.back()->name();
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

    display_file.emplace_back(new cool_gl::Polygon{
        std::move(points), cool_gl::Colour{0.0, 0.0, 0.0}, std::move(name)});

    auto row = *object_list->append();
    row[my_columns.column_type] = display_file.back()->type();
    row[my_columns.column_name] = display_file.back()->name();
  } else if (command_string == "translate") {
    std::string name;

    std::string dx_string;
    std::string dy_string;
    std::string dz_string;

    entrie_stream >> name >> dx_string >> dy_string >> dz_string;

    double dx = std::stod(dx_string);
    double dy = std::stod(dy_string);
    double dz = std::stod(dz_string);

    auto translate_transform = cool_gl::create_translate_transform(dx, dy, dz);

    apply_transform(name, translate_transform);
  } else if (command_string == "scale") {
    std::string name;

    std::string sx_string;
    std::string sy_string;
    std::string sz_string;

    entrie_stream >> name >> sx_string >> sy_string >> sz_string;

    double sx = std::stod(sx_string);
    double sy = std::stod(sy_string);
    double sz = std::stod(sz_string);

    auto scale_transform = cool_gl::create_scale_transform(sx, sy, sz);

    apply_transform_on_mass_centre(name, scale_transform);
  } else if (command_string == "rotate") {
    std::string name;

    std::string rad_string;

    entrie_stream >> name >> rad_string;

    double rad = std::stod(rad_string);

    auto rotate_transform = cool_gl::create_rotate_transform(rad);

    apply_transform_on_mass_centre(name, rotate_transform);
  } else if (command_string == "rotate_at") {
    std::string name;

    std::string rotation_centre_x_string;
    std::string rotation_centre_y_string;

    std::string rad_string;

    entrie_stream >> name >> rotation_centre_x_string >>
        rotation_centre_y_string >> rad_string;

    double rotation_centre_x = std::stod(rotation_centre_x_string);
    double rotation_centre_y = std::stod(rotation_centre_y_string);

    auto translate_to_rotation_centre = cool_gl::create_translate_transform(
        -1 * rotation_centre_x, -1 * rotation_centre_y, 0.0);

    auto translate_from_rotation_centre = cool_gl::create_translate_transform(
        rotation_centre_x, rotation_centre_y, 0.0);

    double rad = std::stod(rad_string);

    auto rotate_transform = cool_gl::create_rotate_transform(rad);

    auto final_transform =
        cool_gl::multiply(translate_to_rotation_centre, rotate_transform);

    final_transform =
        cool_gl::multiply(final_transform, translate_from_rotation_centre);

    apply_transform(name, final_transform);

  } else if (command_string == "clip_line_cohen_sutherland") {
    cool_gl::line_clipping_type = cool_gl::LineClippingType::cohen_sutherland;
  } else if (command_string == "clip_line_liang_barsky") {
    cool_gl::line_clipping_type = cool_gl::LineClippingType::liang_barsky;
  }
    else {
    output_stream
        << "Please enter a valid command! Enter help to see valid commands."
        << std::endl;
  }

  print_to_cool_main_entry_text_view_output(output_stream);
  cool_main_entry->set_text("");

  cool_drawing_area->signal_draw().connect(
      sigc::mem_fun(this, &CoolApp::cool_drawing_area_draw));
  cool_drawing_area->queue_draw();
}

  int CoolApp::run() {
    return gtk_application->run(*cool_main_application_window);
  }

} // namespace cool_app
