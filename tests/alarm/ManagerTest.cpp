#include <memory>

#include "AlarmData.hpp"
#include "alarm/Alarm.hpp"
#include "alarm/Manager.hpp"
#include "catch2/catch_test_macros.hpp"

// Tests
// Disable alarm
// Get correct next time with disabled alarm
// Can serialise alarms

TEST_CASE("Manager Test") {
  pie_alarm::alarm::Manager manager;

  SECTION("GetAlarms returns empty with no alarms") {
    REQUIRE(manager.GetAlarmIds().size() == 0);
  }

  auto testActivity = std::make_unique<TestActivity>();
  pie_alarm::alarm::Alarm alarm(alarmTime, alarmDays, std::move(testActivity));

  SECTION("Add an alarm adds to GetAlarms List") {
    int64_t const alarmId = manager.AddAlarm(std::move(alarm));

    const std::vector<int64_t> alarms = manager.GetAlarmIds();
    REQUIRE(alarms.size() == 1);
    REQUIRE(std::count(alarms.begin(), alarms.end(), alarmId) > 0);
  }

  SECTION("Remove an alarm from the list") {
    int64_t const alarmId = manager.AddAlarm(std::move(alarm));
    manager.RemoveAlarm(alarmId);
    REQUIRE(manager.GetAlarmIds().size() == 0);
  }

  auto testScheduler = std::make_shared<TestScheduler>();

  SECTION("Adding alarm schedules it") {
    pie_alarm::alarm::Manager manager{testScheduler};
    manager.AddAlarm(std::move(alarm));
    REQUIRE(testScheduler->GetRunningJobs().size() == 1);
  }

  SECTION("Removing alarm unschedules it") {
    pie_alarm::alarm::Manager manager{testScheduler};
    int64_t const alarmId = manager.AddAlarm(std::move(alarm));
    manager.RemoveAlarm(alarmId);
    REQUIRE(testScheduler->GetRunningJobs().size() == 0);
  }

  SECTION("Can get next alarm time") {
    using namespace std::chrono;

    auto nextTime = alarm.FindNextTime(system_clock::now());

    manager.AddAlarm({hh_mm_ss{hours(8) + minutes{50}}, alarmDays,
                      std::make_unique<TestActivity>()});

    // Alarm is same as above but at 8.20am
    manager.AddAlarm(std::move(alarm));

    manager.AddAlarm({hh_mm_ss{hours(8) + minutes{30}}, alarmDays,
                      std::make_unique<TestActivity>()});

    REQUIRE(manager.GetNextAlarmTime() == nextTime);
  }
}