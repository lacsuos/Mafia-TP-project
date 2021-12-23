add_library(main STATIC
        src/mainwindow.cpp
        src/fragmentnavigator.cpp
        )
target_include_directories(main PUBLIC include)
target_link_libraries(main PUBLIC Qt${QT_VERSION_MAJOR}::Widgets abstract)
