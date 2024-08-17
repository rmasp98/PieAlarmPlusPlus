#ifndef PIEALARM_SCHEDULER_HPP
#define PIEALARM_SCHEDULER_HPP

#include <chrono>
#include <functional>
#include <mutex>
#include <unordered_set>

#include "Timer.hpp"

namespace pie_alarm::utils {

class Scheduler {
 public:
  using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

  Scheduler() = default;
  ~Scheduler() = default;

  std::unordered_set<int64_t> GetRunningJobs() const;
  virtual int64_t AddJob(TimePoint const& time,
                         std::function<void()> const& callback);
  void RemoveJob(int64_t id);

 protected:
  void ClearFinishedJobs();

 private:
  std::unordered_map<int64_t, Timer> jobs_;
  inline static std::atomic<int64_t> idCounter_ = 0;
  mutable std::mutex mutex_;
};

}  // namespace pie_alarm::utils

#endif  // PIEALARM_SCHEDULER_HPP