#include "Timer.hpp"

namespace pie_alarm::utils {

Timer::Timer(std::chrono::milliseconds const& interval,
             std::function<void()> const& callback, bool repeat) {
  thread_ = std::thread([this, interval, callback, repeat]() {
    while (!exit_) {
      {
        std::unique_lock<std::mutex> lock(mutex_);
        if (exit_) return;

        // Condition variable prevents having to wait in destructor
        // Note: wait_for releases lock
        conditionVariable_.wait_for(lock, interval);
      }

      if (exit_) return;
      callback();

      if (!repeat) exit_ = true;
    }
  });
}

Timer::~Timer() {
  Kill();

  if (thread_.joinable()) {
    thread_.join();
  }
}

void Timer::Kill() {
  // Prevents race condition of notify happening before wait_for
  std::unique_lock<std::mutex> lock(mutex_);
  exit_ = true;
  conditionVariable_.notify_all();
}

}  // namespace pie_alarm::utils