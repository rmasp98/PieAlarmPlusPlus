#include <gtkmm/application.h>

#include "ui/Window.hpp"

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("Pie Alarm");

  // Shows the window and returns when it is closed.
  return app->make_window_and_run<pie_alarm::ui::Window>(argc, argv);
}