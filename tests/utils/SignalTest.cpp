
#include "catch2/catch_test_macros.hpp"
#include "utils/Signal.hpp"

TEST_CASE("Signal Tests") {
  sigc::signal<void()> signal;
  pie_alarm::utils::SignalProxy<void()> proxy(signal);

  SECTION("Can connect to a signal") {
    bool HasWorked = false;
    auto const callback = [&HasWorked]() { HasWorked = true; };
    proxy.Connect(callback);
    signal.emit();

    REQUIRE(HasWorked);
  }

  SECTION("Can connect to a multiple to signal") {
    bool HasWorked1 = false;
    bool HasWorked2 = false;
    auto const callback = [&HasWorked1]() { HasWorked1 = true; };
    proxy.Connect(callback);
    proxy.Connect([&HasWorked2]() { HasWorked2 = true; });
    signal.emit();

    REQUIRE((HasWorked1 && HasWorked2));
  }

  SECTION("Can disconnect from signal afterwards") {
    bool HasWorked = true;
    auto const callback = [&HasWorked]() { HasWorked = false; };
    auto connection = proxy.Connect(callback);
    connection.disconnect();
    signal.emit();

    REQUIRE(HasWorked);
  }
}