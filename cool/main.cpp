#include "cool_app.h"
#include "model_columns.h"
#include <cool_gl/cool_gl.h>
#include <gtkmm.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

int main(int argc, char **argv) { return cool_app::CoolApp{}.run(argc, argv); }
