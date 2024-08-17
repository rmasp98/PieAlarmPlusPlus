#include "Alarm.hpp"

namespace pie_alarm::alarm {

nlohmann::json Alarm::Serialise() const {
  return {{"hour", time_.hours().count()},
          {"minute", time_.minutes().count()},
          {"activity", activity_.Serialise()},
          {"is_active", IsActive()}};
}

void Alarm::Schedule(utils::Scheduler& scheduler) {
  TimePoint const& nextAlarmTime =
      FindNextTime(std::chrono::system_clock::now());
  ScheduleInternal(nextAlarmTime, scheduler);
}

void Alarm::Deactivate(utils::Scheduler& scheduler) {
  activity_.Stop();
  scheduler.RemoveJob(currentJob_);
  currentJob_ = -1;
}

void Alarm::Snooze(int32_t snoozeMinutes, utils::Scheduler& scheduler) {
  activity_.Stop();
  TimePoint const& nextAlarmTime =
      std::chrono::system_clock::now() + std::chrono::minutes(snoozeMinutes);
  ScheduleInternal(nextAlarmTime, scheduler);
}

Alarm::TimePoint Alarm::FindNextTime(
    Alarm::TimePoint const& nowDateTime) const {
  using Days = std::chrono::days;

  auto const& nowDay = std::chrono::floor<Days>(nowDateTime);
  Time const nowTime{
      std::chrono::duration_cast<std::chrono::minutes>(nowDateTime - nowDay)};
  Weekday const nowWeekday{std::chrono::sys_days(nowDay)};

  Days diffDays{8};
  if (!days_.empty()) {
    for (auto const& alarmDay : days_) {
      auto newDiff = alarmDay - nowWeekday;

      // If we have already passed this alarm, get next weeks
      if (newDiff == Days{0} && nowTime.to_duration() > time_.to_duration()) {
        newDiff += Days{7};
      }

      if (newDiff < diffDays) {
        diffDays = newDiff;
      }
    }
  } else {
    diffDays = nowTime.to_duration() > time_.to_duration() ? Days{1} : Days{0};
  }

  return nowDay + diffDays + time_.hours() + time_.minutes();
}

void Alarm::ScheduleInternal(TimePoint const& time,
                             utils::Scheduler& scheduler) {
  if (IsActive()) {
    Deactivate(scheduler);
  }

  currentJob_ = scheduler.AddJob(time, [this]() { activity_.Start(); });
}

}  // namespace pie_alarm::alarm