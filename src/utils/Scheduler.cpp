#include "Scheduler.hpp"

namespace pie_alarm::utils {

std::unordered_set<int64_t> Scheduler::GetRunningJobs() const {
  std::unique_lock<std::mutex> lock(mutex_);
  std::unordered_set<int64_t> keys;
  for (auto const& job : jobs_) {
    keys.insert(job.first);
  }
  return keys;
}

int64_t Scheduler::AddJob(TimePoint const& time,
                          std::function<void()> const& callback) {
  auto id = idCounter_++;

  auto timeUntil = std::chrono::duration_cast<std::chrono::milliseconds>(
      time - std::chrono::system_clock::now());

  {
    std::unique_lock<std::mutex> lock(mutex_);
    jobs_.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                  std::forward_as_tuple(timeUntil, callback, false));
  }

  ClearFinishedJobs();
  return id;
}

void Scheduler::RemoveJob(int64_t id) {
  std::unique_lock<std::mutex> lock(mutex_);
  jobs_.erase(id);
}

void Scheduler::ClearFinishedJobs() {
  std::unique_lock<std::mutex> lock(mutex_);
  for (auto job = jobs_.begin(); job != jobs_.end();) {
    if (!job->second.IsRunning()) {
      job = jobs_.erase(job);
    } else {
      ++job;
    }
  }
}

}  // namespace pie_alarm::utils