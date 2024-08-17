#ifndef PIEALARM_ALARM_HPP
#define PIEALARM_ALARM_HPP

#include <chrono>
#include <vector>

#include "Activity.hpp"
#include "nlohmann/json.hpp"
#include "utils/Scheduler.hpp"

namespace pie_alarm::alarm {

class Alarm {
 public:
  using Time = std::chrono::hh_mm_ss<std::chrono::minutes>;
  using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
  using Weekday = std::chrono::weekday;

  Alarm(Time const& time, std::vector<Weekday> const& days, Activity& activity)
      : time_(time), days_(days), activity_(activity) {}
  ~Alarm() = default;

  nlohmann::json Serialise() const;

  void Schedule(utils::Scheduler&);
  void Deactivate(utils::Scheduler&);
  void Snooze(int32_t snoozeMinutes, utils::Scheduler&);
  bool IsActive() const { return currentJob_ >= 0; }

  TimePoint FindNextTime(TimePoint const& nowDateTime) const;

 protected:
  void ScheduleInternal(TimePoint const&, utils::Scheduler&);

 private:
  Time const time_;
  std::vector<Weekday> const days_;
  Activity& activity_;
  int64_t currentJob_ = -1;
};

}  // namespace pie_alarm::alarm

#endif  // PIEALARM_ALARM_HPP