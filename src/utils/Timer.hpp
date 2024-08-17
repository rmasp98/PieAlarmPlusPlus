#ifndef PIEALARM_TIMER_HPP
#define PIEALARM_TIMER_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace pie_alarm::utils {

class Timer {
 public:
  Timer(std::chrono::milliseconds const& interval,
        std::function<void()> const& callback, bool repeat = true);

  ~Timer();

  void Kill();

  bool IsRunning() const { return !exit_; }

 private:
  std::thread thread_;
  bool exit_ = false;
  std::condition_variable conditionVariable_;
  std::mutex mutex_;
};

}  // namespace pie_alarm::utils

#endif  // PIEALARM_TIMER_HPP