cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 COMPONENTS Widgets Multimedia MultimediaWidgets Network Gui Core REQUIRED)

if(APPLE)
    set(Qt5_DIR /usr/local/opt/qt@5/lib/cmake/Qt5)
endif()

add_executable(main main.cpp)
target_link_libraries(main
                      Qt${QT_VERSION_MAJOR}::Widgets
                      Qt${QT_VERSION_MAJOR}::Multimedia
                      Qt${QT_VERSION_MAJOR}::MultimediaWidgets
                      Qt${QT_VERSION_MAJOR}::Network
                      Qt${QT_VERSION_MAJOR}::Gui
                      Qt${QT_VERSION_MAJOR}::Core
                      )