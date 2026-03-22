#ifndef PIEALARM_ALARMITEM_HPP
#define PIEALARM_ALARMITEM_HPP

#include <gtkmm/button.h>

#include "alarm/Alarm.hpp"

namespace pie_alarm::ui {

class AlarmItem : public Gtk::Button {
 public:
  AlarmItem(const alarm::Alarm& alarm);
  ~AlarmItem() override = default;

 protected:
  const alarm::Alarm& alarm_;
};

}  // namespace pie_alarm::ui

#endif  // PIEALARM_ALARMITEM_HPP