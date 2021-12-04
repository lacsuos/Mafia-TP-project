add_library(messages STATIC src/messages.cpp)
target_include_directories(messages PUBLIC include)
target_link_libraries(messages ${Boost_LIBRARIES})

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()