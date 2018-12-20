project("Pipes" LANGUAGES CXX)
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

find_package(Boost COMPONENTS chrono graph)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(source)

find_package(GTest REQUIRED)
enable_testing()
add_subdirectory(tests)

