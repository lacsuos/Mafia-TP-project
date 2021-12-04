project(Mafia-TP-project)
cmake_minimum_required(VERSION 3.11)
include_directories(Player GameRoom)

add_library(GAME SHARED
        Player/Player.cpp Player/Player.h
        Player/Citizen.cpp Player/Citizen.h
        Player/GameHost.cpp Player/GameHost.h
        Player/Mafia.cpp Player/Mafia.h
        GameRoom/GameRoom.cpp GameRoom/GameRoom.h
        )

add_executable(Mafia-TP-project main.cpp)
target_link_libraries(Mafia-TP-project GAME)
#enable_testing()
#find_package(GTest REQUIRED)

#include_directories(${GTEST_INCLUDE_DIRS})
#message(${GTEST_LIBRARIES})
#message(${GTEST_INCLUDE_DIRS})

#add_executable(tests
        #tests/main.cpp
       # tests/GameEntities.cpp
        #tests/Player.cpp
      #  tests/Utilities.cpp
      #  )

#target_link_libraries(tests ${GTEST_LIBRARIES} pthread Logger GameEntities Player ConfigurationHandler)

#if(NOT TARGET spdlog)
    # Stand-alone build
  #  find_package(spdlog REQUIRED)
#endif()

#add_library(logic_lib INTERFACE)
#target_link_libraries(logic_lib INTERFACE spdlog::spdlog Logger GameEntities Player ConfigurationHandler)