#include <cool_gl/Window.h>

namespace cool_gl {

Window::Window(double height, double width, Vec centre, Vec view_up) noexcept
    : height{height}, width{width}, centre{centre}, view_up{view_up} {}
}
