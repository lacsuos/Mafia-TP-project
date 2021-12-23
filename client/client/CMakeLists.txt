project(Client CXX)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Werror -Wall -Wpedantic)

find_package(Boost 1.65.1 REQUIRED COMPONENTS system log thread)
find_package(Threads)

include_directories(SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})

add_executable(client_prog main.cpp)

add_subdirectory(lib)

target_link_libraries(client_prog client_lib messages
        ${CMAKE_THREAD_LIBS_INIT})