# cmake minimum version
cmake_minimum_required(VERSION 3.16)

# project name and version
project(small_log 
        VERSION 1.0 
        DESCRIPTION "A small footprint logging library - no heap usage"
        LANGUAGES CXX)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# add executable
add_library(small_log 
        src/slog.cpp
        src/slog.h
        src/timedate.cpp
        src/timedate.h)

# add include directories
target_include_directories(small_log PUBLIC ${PROJECT_SOURCE_DIR}/src)


# unit tests
enable_testing()

find_package(GTest REQUIRED)

add_executable(unit_tests 
        test/test_slog.cpp)

target_link_libraries(unit_tests GTest::gtest GTest::gtest_main small_log)

target_include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_test(test_all unit_tests)

