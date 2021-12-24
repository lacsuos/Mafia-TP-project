add_library(resolver STATIC src/resolver.cpp)
target_include_directories(resolver PUBLIC include)
target_link_libraries(resolver PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
        PUBLIC fragments
        PUBLIC abstract
        PUBLIC client_lib
        PUBLIC client-impl)
