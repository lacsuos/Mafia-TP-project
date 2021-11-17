cmake_minimum_required(VERSION 3.9.2)
project(Mafia_TP_project)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(
        -Werror
        -Wall
        -Wextra
        -Wpedantic
)
if(TEST_SEQUENCE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

add_executable(Mafia_TP_project sources/main.c)

#set(STATIC_LIB_SOURCE_FILES ./sources/readAndCloseFile.c ./sources/findSequence.c sources/findAllSequenceOneProcess.c)
#add_library(one_process_lib STATIC ${STATIC_LIB_SOURCE_FILES})

#set(DYNAMIC_LIB_SOURCE_FILES ./sources/readAndCloseFile.c ./sources/findSequence.c sources/findAllSequenceMultiProcess.c)
#add_library(multi_process_lib SHARED ${DYNAMIC_LIB_SOURCE_FILES})

#include_directories(headers)

#add_executable(multi_process.out sources/main.c)
#target_link_libraries(multi_process.out multi_process_lib)

#add_executable(one_process.out sources/main.c)
#target_link_libraries(one_process.out one_process_lib)

#enable_testing()
#find_package(GTest REQUIRED)

#add_executable(test_oneProcess tests/test_oneProcess.cpp)
#target_link_libraries(test_oneProcess GTest::Main one_process_lib)

#add_executable(test_multiProcess tests/test_multiProcess.cpp)
#target_link_libraries(test_multiProcess GTest::Main multi_process_lib)