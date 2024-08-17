include(FetchContent)

FetchContent_Declare(
    Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.6.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(
  unit_tests
  utils/SignalTest.cpp
  utils/TimerTest.cpp
  utils/SchedulerTest.cpp
  alarm/AlarmTest.cpp
)

target_link_libraries(unit_tests
  PRIVATE
    Catch2::Catch2WithMain
    LibPieAlarm
)

#Add "-fsanitize=thread -fPIE -pie -g" for data race testing
# target_compile_features(unit_tests PRIVATE cxx_std_20)
set_target_properties(unit_tests
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   target_compile_options(unit_tests PRIVATE -Wall -Wextra -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
   target_compile_options(unit_tests PRIVATE /W4 /WX /EHsc)
endif()