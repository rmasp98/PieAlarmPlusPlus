#ifndef PIEALARM_TIMER_H
#define PIEALARM_TIMER_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace pie_alarm::utils {

class Timer {
 public:
  Timer(std::chrono::milliseconds interval, std::function<void()> callback,
        bool repeat = true);

  ~Timer();

  void KillTimer() { exit_ = true; }

  bool IsTimerRunning() const { return !exit_; }

 private:
  std::thread thread_;
  bool exit_ = false;
  std::condition_variable conditionVariable_;
  std::mutex mutex_;
};

}  // namespace pie_alarm::utils

#endif  // PIEALARM_TIMER_H