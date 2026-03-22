#include "Manager.hpp"

#include <unordered_set>

namespace pie_alarm::alarm {

std::vector<int64_t> Manager::GetAlarmIds() const {
  std::vector<int64_t> returnValue;
  for (auto const& alarm : alarms_) {
    returnValue.push_back(alarm.GetId());
  }
  return returnValue;
}

int64_t Manager::AddAlarm(Alarm&& alarm) {
  assert(scheduler_);

  alarm.SetScheduler(scheduler_);
  alarm.Schedule();
  int64_t const id = alarm.GetId();

  alarms_.emplace(std::move(alarm));

  return id;
}

void Manager::RemoveAlarm(int64_t alarmId) {
  std::erase_if(alarms_, [alarmId](Alarm const& alarm) {
    return alarm.GetId() == alarmId;
  });
}

Alarm::TimePoint Manager::GetNextAlarmTime() const {
  Alarm::TimePoint nextTime;
  auto const nowTime = std::chrono::system_clock::now();
  for (auto const& alarm : alarms_) {
    auto const currentAlarmNextTime = alarm.FindNextTime(nowTime);
    if (nextTime == Alarm::TimePoint{} || currentAlarmNextTime < nextTime) {
      nextTime = currentAlarmNextTime;
    }
  }

  return nextTime;
}

}  // namespace pie_alarm::alarm
