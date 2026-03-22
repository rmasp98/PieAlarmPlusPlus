#include "Alarm.hpp"

namespace pie_alarm::alarm {
Alarm::Alarm(Time const& time, std::vector<Weekday> const& days,
             const std::shared_ptr<Activity>& activity)
    : id_(ID_COUNTER++), time_(time), days_(days), activity_(activity) {
  if (activity_) {
    activityCompleteConnection_ = activity_->SignalComplete().Connect(
        sigc::mem_fun(*this, &Alarm::OnActivityCompleted));
  }
}

Alarm::Alarm(Alarm&& other)
    : id_(other.id_),
      time_(other.time_),
      days_(other.days_),
      activity_(std::move(other.activity_)),
      currentJob_(other.currentJob_),
      weakScheduler_(other.weakScheduler_) {
  // TODO: if active, we should probably deactivate and reschedule the alarm
  if (activity_) {
    other.activityCompleteConnection_.disconnect();
    activityCompleteConnection_ = activity_->SignalComplete().Connect(
        sigc::mem_fun(*this, &Alarm::OnActivityCompleted));
  }
}

nlohmann::json Alarm::Serialise() const {
  return {{"hour", time_.hours().count()},
          {"minute", time_.minutes().count()},
          {"activity", activity_ ? activity_->Serialise() : "None"},
          {"is_active", IsActive()}};
}

void Alarm::Schedule() {
  TimePoint const& nextAlarmTime =
      FindNextTime(std::chrono::system_clock::now());
  ScheduleInternal(nextAlarmTime);
}

void Alarm::Deactivate() {
  if (activity_) activity_->Stop(true);
  if (auto scheduler = weakScheduler_.lock()) scheduler->RemoveJob(currentJob_);

  currentJob_ = -1;
}

void Alarm::Snooze(int32_t snoozeMinutes) {
  if (activity_) activity_->Stop();
  TimePoint const& nextAlarmTime =
      std::chrono::system_clock::now() + std::chrono::minutes(snoozeMinutes);
  ScheduleInternal(nextAlarmTime);
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

void Alarm::ScheduleInternal(TimePoint const& time) {
  if (IsActive()) Deactivate();

  if (auto scheduler = weakScheduler_.lock()) {
    // If we make this any more complex, we need to update the move constructor
    // to reschedule alarm
    currentJob_ = scheduler->AddJob(
        time, [weakActivity = std::weak_ptr<Activity>(activity_)]() {
          if (auto activity = weakActivity.lock()) activity->Start();
        });
  }
}

void Alarm::OnActivityCompleted() { activityCompleteSignal_.emit(0); }

}  // namespace pie_alarm::alarm