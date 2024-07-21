cmake_minimum_required(VERSION 3.5.0)
project(PieAlarm)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-4.0)

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(PieAlarm
    src/main.cpp
    src/ui/Window.cpp
    src/ui/main/MainPage.cpp
    src/utils/Timer.cpp
)

target_link_libraries(PieAlarm
    ${GTKMM_LIBRARIES}  )