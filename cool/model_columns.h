#pragma once

class ModelColumns : public Gtk::TreeModelColumnRecord
{
public:
  ModelColumns() {
    add(column_type);
    add(column_name);
  }

  Gtk::TreeModelColumn<std::string> column_type;
  Gtk::TreeModelColumn<std::string> column_name;
};
