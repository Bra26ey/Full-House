cmake_minimum_required(VERSION 3.19)
project(Full_House)

set(CMAKE_CXX_STANDARD 14)

include_directories(logic/Player logic/Utilities logic/GameEntities)

add_executable(logic
        logic/main.cpp
        logic/Player/Player.cpp
        logic/Player/HumanPlayer.cpp
        logic/GameEntities/HandProcess.cpp
        logic/GameEntities/Card.cpp)

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})
message(${GTEST_INCLUDE_DIRS})

add_executable(tests
        logic/tests/main.cpp
        logic/tests/Player.cpp
        logic/tests/Utilities.cpp
        logic/tests/GameEntities.cpp
        logic/Player/Player.cpp
        logic/Player/HumanPlayer.cpp
        logic/Utilities/FileLogger.cpp
        logic/GameEntities/Card.cpp
        logic/GameEntities/HandProcess.cpp)

target_link_libraries(tests ${GTEST_LIBRARIES} pthread)