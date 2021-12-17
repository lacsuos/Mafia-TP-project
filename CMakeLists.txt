project(Mafia-TP-project)
cmake_minimum_required(VERSION 3.11)
add_compile_options(
        -Werror
        -Wall
        -Wextra
        -Wpedantic
)
#if(TEST_SEQUENCE)
 #   add_compile_options(--coverage)
 #   add_link_options(--coverage)
#endif()
include_directories(Player PlayRoom)

add_library(GAME SHARED
        Player/Player.cpp Player/Player.h
        Player/Citizen.cpp Player/Citizen.h
        Player/GameHost.cpp Player/GameHost.h
        Player/Mafia.cpp Player/Mafia.h
        PlayRoom/PlayRoom.cpp PlayRoom/PlayRoom.h
        )

add_executable(Mafia-TP-project main.cpp)
target_link_libraries(Mafia-TP-project GAME)
enable_testing()
find_package(GTest REQUIRED)

add_executable(test_RoomInfo tests/testRoomInfo.cpp)
target_link_libraries(test_RoomInfo GTest::Main GAME)