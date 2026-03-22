#ifndef PIEALARM_MAINPAGE_HPP
#define PIEALARM_MAINPAGE_HPP

#include <gtkmm/builder.h>

#include "ui/Page.hpp"
#include "utils/Timer.hpp"

namespace pie_alarm::ui {

class MainPage : public Page {
 public:
  MainPage();
  ~MainPage() override = default;

 protected:
  void UpdateTime();
  void OnAlarmClicked();

 private:
  Glib::RefPtr<Gtk::Builder> builder_;
  utils::Timer timer_;
};

}  // namespace pie_alarm::ui

#endif  // PIEALARM_MAINPAGE_HPP