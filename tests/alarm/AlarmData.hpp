#include <thread>

#include "alarm/Activity.hpp"
#include "utils/Scheduler.hpp"

using namespace std::chrono;

std::chrono::hh_mm_ss const alarmTime{hours(8) + minutes(20)};
std::vector<weekday> const alarmDays{Tuesday, Friday};

class TestActivity : public pie_alarm::alarm::Activity {
 public:
  virtual ~TestActivity() = default;

  void Start() override {
    HasRun = true;
    std::this_thread::sleep_for(1ms);
    Stop();
  }
  void Stop(bool ignoreCallback = false) override {
    HasStopped = true;
    pie_alarm::alarm::Activity::Stop(ignoreCallback);
  }
  nlohmann::json Serialise() const override { return "test"; };

  bool HasRun = false;
  bool HasStopped = false;
};

class TestScheduler : public pie_alarm::utils::Scheduler {
 public:
  ~TestScheduler() = default;

  int64_t AddJob(TimePoint const& time,
                 std::function<void()> const& callback) override {
    JobTime = time;
    TimePoint const Immediately = system_clock::now() + 1ms;
    return pie_alarm::utils::Scheduler::AddJob(Immediately, callback);
  }

  TimePoint JobTime;
};