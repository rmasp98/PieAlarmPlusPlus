#ifndef PIEALARM_WINDOW_H
#define PIEALARM_WINDOW_H

#include <gtkmm/cssprovider.h>
#include <gtkmm/window.h>

#include "main/MainPage.hpp"

namespace pie_alarm::ui {

class Window : public Gtk::Window {
 public:
  Window();
  virtual ~Window() = default;

 protected:
  void OnButtonClicked();

  MainPage page_;
  Glib::RefPtr<Gtk::CssProvider> cssProvider_;
};

}  // namespace pie_alarm::ui

#endif  // PIEALARM_WINDOW_H