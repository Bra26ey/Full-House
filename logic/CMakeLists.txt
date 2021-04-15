cmake_minimum_required(VERSION 3.16)
project(Full_House)

set(CMAKE_CXX_STANDARD 14)

include_directories(Player Utilities GameEntities)


add_library(MyLib SHARED
                         Player/Player.cpp Player/HumanPlayer.cpp Player/HumanPlayer.h Player/Player.h Player/PlayerInterface.h Player/Profile.h
                         GameEntities/HandProcess.cpp GameEntities/Card.cpp GameEntities/Board.h GameEntities/Card.h GameEntities/HandProcess.h
                         Utilities/FileLogger.cpp Utilities/FileLogger.h Utilities/HandConfiguration.h Utilities/Logger.h)

add_executable(logic
        main.cpp)
target_link_libraries(logic MyLib)

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})
message(${GTEST_INCLUDE_DIRS})

add_executable(tests
        tests/main.cpp
        )

target_link_libraries(tests ${GTEST_LIBRARIES} pthread MyLib)
