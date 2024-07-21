#include "Timer.hpp"

namespace pie_alarm::utils {

Timer::Timer(std::chrono::milliseconds interval, std::function<void()> callback,
             bool repeat) {
  thread_ = std::thread([=, this]() {
    while (!exit_) {
      std::unique_lock<std::mutex> lock(mutex_);
      if (exit_) return;

      // Condition variable prevents having to wait in destructor
      conditionVariable_.wait_for(lock, interval);

      if (exit_) return;
      callback();

      if (!repeat) exit_ = true;
    }
  });
}

Timer::~Timer() {
  // Prevents race condition of notify happening before wait_for
  {
    std::unique_lock<std::mutex> lock(mutex_);
    exit_ = true;
    conditionVariable_.notify_all();
  }

  if (thread_.joinable()) {
    thread_.join();
  }
}

}  // namespace pie_alarm::utils