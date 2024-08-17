#include "catch2/catch_test_macros.hpp"
#include "utils/Scheduler.hpp"

TEST_CASE("Scheduler Tests") {
  pie_alarm::utils::Scheduler scheduler;
  auto time = std::chrono::system_clock::now() + std::chrono::milliseconds(5);
  bool isExecuted = false;
  auto callback = [&isExecuted]() { isExecuted = true; };

  SECTION("Can get list of running jobs") {
    REQUIRE(scheduler.GetRunningJobs().empty());
  }

  SECTION("Can add a job to the scheduler") {
    scheduler.AddJob(time, callback);
    REQUIRE(scheduler.GetRunningJobs().size() == 1);
  }

  SECTION("Adding job returns id") {
    auto const id = scheduler.AddJob(time, callback);
    REQUIRE(scheduler.GetRunningJobs().contains(id));
  }

  SECTION("Add multiple jobs returns unique ids") {
    for (int i = 0; i < 10; ++i) {
      scheduler.AddJob(time, callback);
    }
    REQUIRE(scheduler.GetRunningJobs().size() == 10);
  }

  SECTION("Can remove job") {
    auto const id = scheduler.AddJob(time, callback);
    scheduler.RemoveJob(id);
    REQUIRE(scheduler.GetRunningJobs().empty());
  }

  SECTION("Added job executes at time") {
    scheduler.AddJob(time, callback);

    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    REQUIRE(isExecuted == false);
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    REQUIRE(isExecuted == true);
  }

  SECTION("Removed job never executes") {
    auto id = scheduler.AddJob(time, callback);
    scheduler.RemoveJob(id);

    std::this_thread::sleep_for(std::chrono::milliseconds(6));
    REQUIRE(isExecuted == false);
  }

  SECTION("Automatically remove executed jobs on AddJob") {
    scheduler.AddJob(time, callback);
    std::this_thread::sleep_for(std::chrono::milliseconds(7));
    scheduler.AddJob(time, callback);
    REQUIRE(scheduler.GetRunningJobs().size() == 1);
  }
}