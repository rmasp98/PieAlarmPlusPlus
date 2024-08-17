cmake_minimum_required(VERSION 3.11.0)
project(PieAlarm)

# Define if testing should be compiled
set(PIEALARM_BUILD_TESTS CACHE BOOL true)



add_subdirectory(src)

# if ( PIEALARM_BUILD_TESTS )
   enable_testing()
   add_subdirectory(tests)
# endif()