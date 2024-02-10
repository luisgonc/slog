# cmake minimum version
cmake_minimum_required(VERSION 3.16)

# project name and version
project(small_log)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# add executable
add_executable(small_log 
        src/main.cpp
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
        test/test_slog.cpp
        src/slog.cpp
        src/slog.h
        src/timedate.cpp
        src/timedate.h)

target_link_libraries(unit_tests GTest::gtest GTest::gtest_main)

target_include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_test(test_all unit_tests)

