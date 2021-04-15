project(Client CXX)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS "--coverage -Wall -Werror -Wpedantic bjam –with-system –with-thread –with-date_time –with-regex –withserialization stage")

enable_testing()

add_subdirectory(project)
