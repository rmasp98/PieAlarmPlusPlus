#ifndef PIEALARM_WINDOW_HPP
#define PIEALARM_WINDOW_HPP

#include <gtkmm/cssprovider.h>
#include <gtkmm/window.h>

#include <memory>
#include <stack>

#include "Page.hpp"

namespace pie_alarm::ui {

class Window : public Gtk::Window {
 public:
  Window();
  ~Window() override = default;

 protected:
  void OnButtonClicked();
  void OnPageChangeRequested(PageEnum newPage);

  bool SwitchPage(PageEnum newPage);

  std::unique_ptr<Page> CreatePage(PageEnum page);

  std::unique_ptr<Page> currentPage_;
  std::stack<PageEnum> pageHistory_;
  Glib::RefPtr<Gtk::CssProvider> cssProvider_;
};

}  // namespace pie_alarm::ui

#endif  // PIEALARM_WINDOW_HPP