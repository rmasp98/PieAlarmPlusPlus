#ifndef PIEALARM_MAINPAGE_H
#define PIEALARM_MAINPAGE_H

// #include <gtkmm/button.h>
#include <gtkmm/label.h>

#include "../../utils/Timer.hpp"
#include "../Page.hpp"

namespace pie_alarm::ui {

class MainPage : public Page {
 public:
  MainPage();
  virtual ~MainPage() = default;

 protected:
  void UpdateLabel();

 private:
  Gtk::Label* timeLabel_;
  utils::Timer timer_;
};

}  // namespace pie_alarm::ui

#endif  // PIEALARM_MAINPAGE_H