#ifndef PIEALARM_ALARM_MANAGER_HPP
#define PIEALARM_ALARM_MANAGER_HPP

#include <cstdint>
#include <memory>
#include <unordered_set>

#include "Alarm.hpp"
#include "utils/Scheduler.hpp"

namespace pie_alarm::alarm {

class Manager {
 public:
  Manager(const std::shared_ptr<utils::Scheduler>& scheduler =
              std::make_shared<utils::Scheduler>())
      : scheduler_(scheduler) {}

  std::vector<int64_t> GetAlarmIds() const;
  int64_t AddAlarm(Alarm&& alarm);
  void RemoveAlarm(int64_t alarmId);
  Alarm::TimePoint GetNextAlarmTime() const;

 protected:
  std::unordered_set<Alarm, Alarm::HashFunction> alarms_;
  std::shared_ptr<utils::Scheduler> scheduler_;
};

}  // namespace pie_alarm::alarm

#endif  // PIEALARM_ALARM_MANAGER_HPP