#include "catch2/catch_test_macros.hpp"
#include "utils/Timer.hpp"

TEST_CASE("Timer Tests") {
  SECTION("Can trigger callback once") {
    bool hasWorked = false;
    pie_alarm::utils::Timer timer(
        std::chrono::milliseconds{1}, [&hasWorked]() { hasWorked = true; },
        false);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    REQUIRE(hasWorked);
  }

  SECTION("Can trigger callback multiple times") {
    int32_t totalCount = 0;
    pie_alarm::utils::Timer timer(
        std::chrono::milliseconds{1}, [&totalCount]() { totalCount++; }, true);

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    REQUIRE(totalCount > 3);
  }

  SECTION("Can check if timer is running") {
    pie_alarm::utils::Timer timer(std::chrono::milliseconds{1}, []() {}, true);
    REQUIRE(timer.IsRunning());
  }

  SECTION("Can kill the timer") {
    bool hasWorked = true;
    pie_alarm::utils::Timer timer(
        std::chrono::milliseconds{1}, [&hasWorked]() { hasWorked = false; },
        true);
    timer.Kill();

    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    REQUIRE((hasWorked && !timer.IsRunning()));
  }

  SECTION("Destructor kills timer") {
    bool hasWorked = true;
    {
      pie_alarm::utils::Timer timer(
          std::chrono::milliseconds{3}, [&hasWorked]() { hasWorked = false; },
          true);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    REQUIRE(hasWorked);
  }

  SECTION("Destructor immediately kills the timer") {
    auto startTime = std::chrono::high_resolution_clock::now();
    {
      pie_alarm::utils::Timer timer(
          std::chrono::milliseconds{500}, []() {}, true);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime);

    REQUIRE(deltaTime < std::chrono::milliseconds(100));
  }
}