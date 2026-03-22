#include "AlarmListPage.hpp"

#include <gtkmm/button.h>

#include "AlarmItem.hpp"
#include "alarm/Activity.hpp"
#include "alarm/Alarm.hpp"

namespace pie_alarm::ui {

AlarmListPage::AlarmListPage() {
  std::chrono::hh_mm_ss const alarmTime{std::chrono::hours(8) +
                                        std::chrono::minutes(20)};
  std::vector<std::chrono::weekday> const alarmDays{std::chrono::Tuesday,
                                                    std::chrono::Friday};
  // alarm::MusicActivity activity;
  // alarm::Alarm alarm(alarmTime, alarmDays, activity);
  // AlarmItem item(alarm);

  // grid_.attach(item, 0, 0);
  set_child(grid_);
}

}  // namespace pie_alarm::ui