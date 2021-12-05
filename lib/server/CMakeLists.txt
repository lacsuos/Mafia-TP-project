add_library(server_lib STATIC src/server.cpp src/connection.cpp src/game_connection.cpp)
target_include_directories(server_lib PUBLIC include)
target_link_libraries(server_lib ${Boost_LIBRARIES} messages GAME)

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()