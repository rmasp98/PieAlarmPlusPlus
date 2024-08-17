
find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-4.0)

include(FetchContent)

FetchContent_Declare(
    json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG        v3.11.3
)

FetchContent_MakeAvailable(json)

add_library(LibPieAlarm
    ui/Window.cpp
    ui/main/MainPage.cpp

    alarm/Alarm.cpp

    utils/Scheduler.cpp
    utils/Timer.cpp
)

target_include_directories(LibPieAlarm
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
        ${GTKMM_INCLUDE_DIRS}
)

target_link_directories(LibPieAlarm
    PUBLIC
        ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(LibPieAlarm
    PUBLIC
        ${GTKMM_LIBRARIES}
        nlohmann_json::nlohmann_json
)

set_target_properties(LibPieAlarm
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   target_compile_options(LibPieAlarm PRIVATE -Wall -Wextra -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
   target_compile_options(LibPieAlarm PRIVATE /W4 /WX /EHsc)
endif()

add_executable(PieAlarm
    main.cpp
)


target_link_libraries(PieAlarm
    LibPieAlarm
)

set_target_properties(PieAlarm
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   target_compile_options(PieAlarm PRIVATE -Wall -Wextra -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
   target_compile_options(PieAlarm PRIVATE /W4 /WX /EHsc)
endif()