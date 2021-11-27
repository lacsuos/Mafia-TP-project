project(Client CXX)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic")

find_package(Boost 1.65.1 REQUIRED COMPONENTS system log thread)
find_package(Threads)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_library(messages STATIC messages/src/messages.cpp)
target_include_directories(messages PUBLIC messages/include)
target_link_libraries(messages ${Boost_LIBRARIES})

add_library(client_lib STATIC src/client.cpp)
target_include_directories(client_lib PUBLIC include)
target_link_libraries(client_lib ${Boost_LIBRARIES} messages)

add_executable(client_prog main.cpp)

target_link_libraries(client_prog client_lib messages
        ${CMAKE_THREAD_LIBS_INIT})