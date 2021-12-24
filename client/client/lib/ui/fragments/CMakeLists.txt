set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(fragments STATIC
        src/creatingfragment.cpp
        src/gamefragment.cpp
        src/mainfragment.cpp
        src/optionsfragment.cpp
        src/waitingfragment.cpp
        src/screensfactory.cpp
        src/mainwindow.cpp
        src/fragmentnavigator.cpp
        )
target_include_directories(fragments PUBLIC include)
target_link_libraries(fragments PUBLIC Qt${QT_VERSION_MAJOR}::Widgets abstract)
