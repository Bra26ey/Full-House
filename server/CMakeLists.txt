project(Server CXX)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic")

enable_testing()

add_subdirectory(project)
