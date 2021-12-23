add_library(abstract STATIC
        src/abstractfragment.cpp
        src/abstractscreensfactory.cpp
        )
target_include_directories(abstract PUBLIC include)
target_link_libraries(abstract PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
