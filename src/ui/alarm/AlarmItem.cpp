#include "AlarmItem.hpp"

namespace pie_alarm::ui {

AlarmItem::AlarmItem(const alarm::Alarm& alarm) : alarm_(alarm) {
  auto alarmData = alarm_.Serialise();
  set_label(nlohmann::to_string(alarmData));
}

}  // namespace pie_alarm::ui
