add_executable(test_messages src/main.cpp)
target_link_libraries(test_messages messages ${Boost_LIBRARIES} gtest)