#ifndef PIEALARM_ALARM_HPP
#define PIEALARM_ALARM_HPP

#include <chrono>

#include "Activity.hpp"
#include "utils/Scheduler.hpp"

namespace pie_alarm::alarm {

class Alarm {
 public:
  using Time = std::chrono::hh_mm_ss<std::chrono::minutes>;
  using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
  using Weekday = std::chrono::weekday;

  Alarm(Time const& time, std::vector<Weekday> const& days,
        const std::shared_ptr<Activity>& activity);
  ~Alarm() { Deactivate(); }

  Alarm(Alarm&& other);

  bool operator==(const Alarm& other) const { return this->id_ == other.id_; }

  int64_t GetId() const { return id_; }

  void SetScheduler(const std::shared_ptr<utils::Scheduler>& scheduler) {
    weakScheduler_ = scheduler;
  }

  nlohmann::json Serialise() const;

  void Schedule();
  void Deactivate();
  void Snooze(int32_t snoozeMinutes);
  bool IsActive() const { return currentJob_ >= 0; }

  TimePoint FindNextTime(TimePoint const& nowDateTime) const;

  using SignalEvent = pie_alarm::utils::SignalProxy<void(int64_t)>;
  SignalEvent SignalActivityComplete() {
    return SignalEvent(activityCompleteSignal_);
  }

  struct HashFunction {
    size_t operator()(Alarm const& alarm) const {
      return std::hash<int64_t>()(alarm.id_);
    }
  };

 protected:
  void ScheduleInternal(TimePoint const&);
  void OnActivityCompleted();

 private:
  int64_t const id_ = -1;
  Time const time_;
  std::vector<Weekday> const days_;
  std::shared_ptr<Activity> activity_ = nullptr;
  int64_t currentJob_ = -1;

  inline static int64_t ID_COUNTER = 0;
  std::weak_ptr<utils::Scheduler> weakScheduler_;

  sigc::signal<void(int64_t)> activityCompleteSignal_;
  sigc::connection activityCompleteConnection_;
};

}  // namespace pie_alarm::alarm

#endif  // PIEALARM_ALARM_HPP