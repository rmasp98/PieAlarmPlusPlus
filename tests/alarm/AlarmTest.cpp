#include <iostream>

#include "alarm/Activity.hpp"
#include "alarm/Alarm.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json.hpp"
#include "utils/Scheduler.hpp"

using namespace std::chrono;

TEST_CASE("Alarm Tests") {
  std::chrono::hh_mm_ss const alarmTime{hours(8) + minutes(20)};

  std::vector<weekday> const alarmDays{Tuesday, Friday};

  class TestActivity : public pie_alarm::alarm::Activity {
   public:
    void Start() override { HasRun = true; }
    void Stop() override { HasStopped = true; }
    nlohmann::json Serialise() const override { return "test"; };

    bool HasRun = false;
    bool HasStopped = false;
  };

  TestActivity testActivity;
  pie_alarm::alarm::Alarm alarm(alarmTime, alarmDays, testActivity);

  SECTION("Can get alarm data for display") {
    auto actualJson = nlohmann::json::parse(
        R"({"hour": 8, "minute": 20, "activity": "test", "is_active": false})");
    REQUIRE(alarm.Serialise() == actualJson);
  }

  SECTION("Can find next alarm time") {
    pie_alarm::alarm::Alarm const nextAlarm(alarmTime, {Wednesday},
                                            testActivity);

    // This date is a Thursday
    auto const& startDate = sys_days{August / 1 / 2024};

    auto const actualAlarmTime{startDate + days(6) + alarmTime.hours() +
                               alarmTime.minutes()};

    for (int i = 0; i < 7; ++i) {
      auto const& testDate = startDate + days(i);
      auto const& nextAlarmTime = nextAlarm.FindNextTime(testDate + 5h + 35min);

      REQUIRE(nextAlarmTime == actualAlarmTime);
    }
  }

  SECTION("Can find next alarm time when alarm is earlier in the same day") {
    pie_alarm::alarm::Alarm const nextAlarm(alarmTime, {Wednesday},
                                            testActivity);

    // This date is a Wednesday
    auto const& testDate = sys_days{August / 7 / 2024};

    auto const& nextAlarmTime = nextAlarm.FindNextTime(testDate + 18h + 35min);
    auto const actualAlarmTime{testDate + days(7) + alarmTime.hours() +
                               alarmTime.minutes()};

    REQUIRE(nextAlarmTime == actualAlarmTime);
  }

  SECTION("Can find next alarm time when multiple alarm days") {
    pie_alarm::alarm::Alarm const nextAlarm(alarmTime, {Tuesday, Friday},
                                            testActivity);

    // This date is a Wednesday
    auto const& testDate = sys_days{August / 7 / 2024};

    auto const& nextAlarmTime = nextAlarm.FindNextTime(testDate + 18h + 35min);
    auto const actualAlarmTime{testDate + days(2) + alarmTime.hours() +
                               alarmTime.minutes()};

    REQUIRE(nextAlarmTime == actualAlarmTime);
  }

  SECTION("Can find next alarm time when no alarm days") {
    pie_alarm::alarm::Alarm const nextAlarm(alarmTime, {}, testActivity);

    // This date is a Wednesday
    auto const& testDate = sys_days{August / 7 / 2024};

    auto const& nextAlarmTime = nextAlarm.FindNextTime(testDate + 18h + 35min);
    auto const actualAlarmTime{testDate + days(1) + alarmTime.hours() +
                               alarmTime.minutes()};

    REQUIRE(nextAlarmTime == actualAlarmTime);
  }

  SECTION("Can schedule the alarm") {
    pie_alarm::utils::Scheduler scheduler;
    alarm.Schedule(scheduler);
    REQUIRE(alarm.IsActive() == true);
  }

  class TestScheduler : public pie_alarm::utils::Scheduler {
   public:
    int64_t AddJob(TimePoint const& time,
                   std::function<void()> const& callback) override {
      JobTime = time;
      // We run the callback here to confirm correct thing passed in
      callback();
      return {};
    }
    TimePoint JobTime;
  };

  SECTION("Scheduling alarm adds at next alarm time and correct activity job") {
    TestScheduler scheduler;
    alarm.Schedule(scheduler);
    REQUIRE(scheduler.JobTime == alarm.FindNextTime(system_clock::now()));

    // Fake test to make sure activity got executed
    REQUIRE(testActivity.HasRun == true);
  }

  SECTION("Deactivate alarm removes job from scheduler") {
    pie_alarm::utils::Scheduler scheduler;
    alarm.Schedule(scheduler);
    alarm.Deactivate(scheduler);
    REQUIRE(scheduler.GetRunningJobs().empty());
    REQUIRE(alarm.IsActive() == false);
  }

  SECTION("Calling Schedule multiple times doesn't result in multiple jobs") {
    pie_alarm::utils::Scheduler scheduler;
    alarm.Schedule(scheduler);
    alarm.Schedule(scheduler);
    REQUIRE(scheduler.GetRunningJobs().size() == 1);
  }

  SECTION("Snoozing alarm schedules job after snooze time") {
    TestScheduler scheduler;
    const int snoozeTime = 10;
    alarm.Snooze(snoozeTime, scheduler);

    REQUIRE(system_clock::now() + minutes(snoozeTime) - scheduler.JobTime <
            seconds(1));
  }

  SECTION("Snoozing alarm calls stop on activity") {
    pie_alarm::utils::Scheduler scheduler;
    alarm.Snooze(10, scheduler);
    REQUIRE(testActivity.HasStopped == true);
  }

  SECTION("Deactivating alarm calls stop on activity") {
    pie_alarm::utils::Scheduler scheduler;
    alarm.Deactivate(scheduler);
    REQUIRE(testActivity.HasStopped == true);
  }
}