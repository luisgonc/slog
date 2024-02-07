cmake_minimum_required(VERSION 3.16)
project(small_log)

set(CMAKE_CXX_STANDARD 17)

add_executable(small_log src/main.cpp
        src/slog.cpp
        src/slog.h
        src/timedate.cpp
        src/timedate.h)
