add_library(game_room SHARED src/room.cpp)
target_include_directories(game_room PUBLIC include)
target_link_libraries(game_room ${Boost_LIBRARIES})