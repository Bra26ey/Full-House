project(Server CXX)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS "--coverage -Wall -Werror -Wpedantic pthread")

enable_testing()

add_subdirectory(project)
