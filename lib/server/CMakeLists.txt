
add_library(server_lib SHARED src/server.cpp)
target_include_directories(server_lib PUBLIC include)
target_link_libraries(server_lib ${Boost_LIBRARIES})