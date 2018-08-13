cmake_minimum_required(VERSION 3.0)
project(test_spdlog)

set(CMAKE_CXX_STANDARD 14)

find_package(spdlog CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(test_spdlog main.cpp)
target_link_libraries(test_spdlog spdlog::spdlog)