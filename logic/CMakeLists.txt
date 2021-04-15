cmake_minimum_required(VERSION 3.19)
project(Full_House)

set(CMAKE_CXX_STANDARD 14)

include_directories(Player Utilities GameEntities)

add_executable(logic
        main.cpp
        Player/Player.cpp
        Player/HumanPlayer.cpp
        GameEntities/HandProcess.cpp
        GameEntities/Card.cpp)

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})
message(${GTEST_INCLUDE_DIRS})

add_executable(tests
        tests/main.cpp
        tests/Player.cpp
        tests/Utilities.cpp
        tests/GameEntities.cpp
        Player/Player.cpp
        Player/HumanPlayer.cpp
        Utilities/FileLogger.cpp
        GameEntities/Card.cpp
        GameEntities/HandProcess.cpp)

target_link_libraries(tests ${GTEST_LIBRARIES} pthread)