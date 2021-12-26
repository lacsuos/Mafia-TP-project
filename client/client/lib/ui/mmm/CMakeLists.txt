add_library(resolver STATIC src/resolver.cpp)
target_link_libraries(resolver PUBLIC Qt${QT_VERSION_MAJOR}::Widgets fragments abstract client_lib)
target_include_directories(resolver PUBLIC include)
