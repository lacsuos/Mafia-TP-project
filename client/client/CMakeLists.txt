project(Client CXX)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Werror -Wall -Wpedantic)

find_package(Boost 1.65.1 REQUIRED COMPONENTS system log thread)
find_package(Threads)
find_package(Qt5Widgets)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})

add_executable(client_prog main.cpp)

add_subdirectory(lib)

target_link_libraries(client_prog PUBLIC client_lib
        PUBLIC messages
        PUBLIC ${CMAKE_THREAD_LIBS_INIT}
        PUBLIC fragments
        PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)