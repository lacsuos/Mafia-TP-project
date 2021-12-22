add_executable(test_server src/main.cpp)
target_link_libraries(test_server server_lib ${Boost_LIBRARIES} gtest)