#include "Window.hpp"

#include <gtkmm/stylecontext.h>

#include <iostream>

namespace pie_alarm::ui {

Window::Window() : cssProvider_(Gtk::CssProvider::create()) {
  set_title("Basic application");
  set_default_size(800, 480);

  Gtk::StyleContext::add_provider_for_display(
      get_display(), cssProvider_, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  // TODO: figure out how to do paths properly
  cssProvider_->load_from_path("../../src/ui/styles/default.css");

  page_.SignalBack().Connect(sigc::mem_fun(*this, &Window::OnButtonClicked));
  set_child(page_);
}

void Window::OnButtonClicked() { std::cout << "I am clicked" << std::endl; }

}  // namespace pie_alarm::ui