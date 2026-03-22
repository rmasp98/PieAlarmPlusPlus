#ifndef PIEALARM_ALARMLISTPAGE_HPP
#define PIEALARM_ALARMLISTPAGE_HPP

#include <gtkmm/grid.h>

#include "ui/Page.hpp"

namespace pie_alarm::ui {

class AlarmListPage : public Page {
 public:
  AlarmListPage();
  ~AlarmListPage() override = default;

 protected:
  Gtk::Grid grid_;
};

}  // namespace pie_alarm::ui

#endif  // PIEALARM_ALARMLISTPAGE_HPP