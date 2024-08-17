#ifndef PIEALARM_MAINPAGE_HPP
#define PIEALARM_MAINPAGE_HPP

// #include <gtkmm/button.h>
#include <gtkmm/label.h>

#include "ui/Page.hpp"
#include "utils/Timer.hpp"

namespace pie_alarm::ui {

class MainPage : public Page {
 public:
  MainPage();
  virtual ~MainPage() = default;

 protected:
  void UpdateTime();

 private:
  Gtk::Label* timeLabel_;
  utils::Timer timer_;
};

}  // namespace pie_alarm::ui

#endif  // PIEALARM_MAINPAGE_HPP