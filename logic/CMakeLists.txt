cmake_minimum_required(VERSION 3.16)
project(Full_House)

set(CMAKE_CXX_STANDARD 14)

include_directories(Player Utilities GameEntities)



add_library(Logger SHARED
        Utilities/Logger.cpp Utilities/Logger.h)

add_library(ConfigurationHandler SHARED
        Utilities/FileHandler.cpp Utilities/FileHandler.h
        Utilities/ConfigurationHandler.cpp Utilities/ConfigurationHandler.h
        Utilities/HandConfiguration.h
        Player/Player.cpp Player/Player.h)

add_library(GameEntities SHARED
        GameEntities/HandProcess.cpp GameEntities/HandProcess.h
        GameEntities/Card.cpp GameEntities/Card.h
        GameEntities/Board.h
        GameEntities/Deck.cpp GameEntities/Deck.h)

add_library(Player SHARED Player/Player.cpp Player/Player.h
        Player/PlayerInterface.h
        Player/Profile.h)

add_executable(logic
        main.cpp)
target_link_libraries(logic Logger GameEntities Player ConfigurationHandler)

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})
message(${GTEST_INCLUDE_DIRS})

add_executable(tests
        tests/main.cpp
        tests/GameEntities.cpp
        tests/Player.cpp
        tests/Utilities.cpp
        )

target_link_libraries(tests ${GTEST_LIBRARIES} pthread Logger GameEntities Player ConfigurationHandler)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()



# ---------------------------------------------------------------------------------------
# Example of using pre-compiled library
# ---------------------------------------------------------------------------------------
target_link_libraries(logic spdlog::spdlog)

