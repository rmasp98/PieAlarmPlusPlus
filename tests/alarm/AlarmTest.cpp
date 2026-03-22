
#include "AlarmData.hpp"
#include "alarm/Alarm.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json.hpp"

using namespace std::chrono;

// Test
// Need to have a way to present if it is currently triggered
// Handle the case when one time alarm has been triggered

TEST_CASE("Alarm Tests") {
  auto testActivity = std::make_unique<TestActivity>();
  bool& hasRun = testActivity->HasRun;
  bool& hasStopped = testActivity->HasStopped;
  pie_alarm::alarm::Alarm alarm(alarmTime, alarmDays, std::move(testActivity));

  SECTION("Can get alarm data for display") {
    auto actualJson = nlohmann::json::parse(
        R"({"hour": 8, "minute": 20, "activity": "test", "is_active": false})");
    REQUIRE(alarm.Serialise() == actualJson);
  }

  SECTION("Can find next alarm time") {
    pie_alarm::alarm::Alarm const nextAlarm(alarmTime, {Wednesday},
                                            std::make_unique<TestActivity>());

    // This date is a Thursday
    auto const& startDate = sys_days{August / 1 / 2024};

    auto const actualAlarmTime{startDate + days(6) + alarmTime.hours() +
                               alarmTime.minutes()};

    // Tests each day of the week to ensure it works in all cases
    for (int i = 0; i < 7; ++i) {
      auto const& testDate = startDate + days(i);
      auto const& nextAlarmTime = nextAlarm.FindNextTime(testDate + 5h + 35min);

      REQUIRE(nextAlarmTime == actualAlarmTime);
    }
  }

  SECTION("Can find next alarm time when alarm is earlier in the same day") {
    pie_alarm::alarm::Alarm const nextAlarm(alarmTime, {Wednesday},
                                            std::make_unique<TestActivity>());

    // This date is a Wednesday
    auto const& testDate = sys_days{August / 7 / 2024};

    auto const& nextAlarmTime = nextAlarm.FindNextTime(testDate + 18h + 35min);
    auto const actualAlarmTime{testDate + days(7) + alarmTime.hours() +
                               alarmTime.minutes()};

    REQUIRE(nextAlarmTime == actualAlarmTime);
  }

  SECTION("Can find next alarm time when multiple alarm days") {
    pie_alarm::alarm::Alarm const nextAlarm(alarmTime, {Tuesday, Friday},
                                            std::make_unique<TestActivity>());

    // This date is a Wednesday
    auto const& testDate = sys_days{August / 7 / 2024};

    auto const& nextAlarmTime = nextAlarm.FindNextTime(testDate + 18h + 35min);
    auto const actualAlarmTime{testDate + days(2) + alarmTime.hours() +
                               alarmTime.minutes()};

    REQUIRE(nextAlarmTime == actualAlarmTime);
  }

  SECTION("Can find next alarm time when no alarm days") {
    pie_alarm::alarm::Alarm const nextAlarm(alarmTime, {},
                                            std::make_unique<TestActivity>());

    // This date is a Wednesday
    auto const& testDate = sys_days{August / 7 / 2024};

    auto const& nextAlarmTime = nextAlarm.FindNextTime(testDate + 18h + 35min);
    auto const actualAlarmTime{testDate + days(1) + alarmTime.hours() +
                               alarmTime.minutes()};

    REQUIRE(nextAlarmTime == actualAlarmTime);
  }

  auto scheduler = std::make_shared<TestScheduler>();
  alarm.SetScheduler(scheduler);

  SECTION("Can schedule the alarm") {
    alarm.Schedule();
    REQUIRE(alarm.IsActive() == true);
  }

  SECTION("Scheduling alarm adds at next alarm time and correct activity job") {
    alarm.Schedule();
    REQUIRE(scheduler->JobTime == alarm.FindNextTime(system_clock::now()));

    // Fake test to make sure activity got executed
    // TestScheduler should execute activity after 1ms so delay for 2 seconds
    std::this_thread::sleep_for(2ms);
    REQUIRE(hasRun == true);
  }

  SECTION("Deactivate alarm removes job from scheduler") {
    alarm.Schedule();
    alarm.Deactivate();
    REQUIRE(scheduler->GetRunningJobs().empty());
    REQUIRE(alarm.IsActive() == false);
  }

  SECTION("Calling Schedule multiple times doesn't result in multiple jobs") {
    alarm.Schedule();
    alarm.Schedule();
    REQUIRE(scheduler->GetRunningJobs().size() == 1);
  }

  SECTION("Snoozing alarm schedules job after snooze time") {
    const int snoozeTime = 10;
    alarm.Snooze(snoozeTime);

    REQUIRE(system_clock::now() + minutes(snoozeTime) - scheduler->JobTime <
            seconds(1));
  }

  SECTION("Snoozing alarm calls stop on activity") {
    alarm.Snooze(10);
    REQUIRE(hasStopped == true);
  }

  SECTION("Deactivating alarm calls stop on activity") {
    alarm.Deactivate();
    REQUIRE(hasStopped == true);
  }

  SECTION("Completing activity calls on complete in alarm") {
    bool isCalled = false;
    alarm.SignalActivityComplete().Connect(
        [&isCalled](int64_t) { isCalled = true; });

    // Test activity is set to complete after 1ms
    alarm.Schedule();

    // TestScheduler should execute activity after 1ms so delay for 2 seconds
    std::this_thread::sleep_for(2ms);
    REQUIRE(isCalled == true);
  }

  SECTION("Deactivating alarm does not call complete callback") {
    bool isCalled = false;
    alarm.SignalActivityComplete().Connect(
        [&isCalled](int64_t) { isCalled = true; });

    alarm.Schedule();
    alarm.Deactivate();

    REQUIRE(isCalled == false);
  }
}