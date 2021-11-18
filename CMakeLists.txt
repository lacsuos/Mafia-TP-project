project(Server CXX)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic")

find_package(Boost 1.65.1 REQUIRED COMPONENTS system log thread)
find_package(Threads)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_subdirectory(lib)

add_executable(server_prog src/main.cpp)

target_link_libraries(server_prog server_lib
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES})