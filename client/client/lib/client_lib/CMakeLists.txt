add_library(client_lib STATIC src/client.cpp)
target_include_directories(client_lib PUBLIC include)
target_link_libraries(client_lib ${Boost_LIBRARIES} messages)