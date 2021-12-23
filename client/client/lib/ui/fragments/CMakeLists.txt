add_library(fragments STATIC
        src/creatingfragment.cpp
        src/gamefragment.cpp
        src/mainfragment.cpp
        src/optionsfragment.cpp
        src/waitingfragment.cpp
        src/screensfactory.cpp
        src/mainwindow.cpp
        src/fragmentnavigator.cpp
        src/abstractfragment.cpp
        src/abstractscreensfactory.cpp
        )
target_link_libraries(fragments PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(fragments PUBLIC include)
