include_directories(Player Utilities GameEntities)



add_library(Logger SHARED
        Utilities/Logger.cpp Utilities/Logger.h)

add_library(ConfigurationHandler SHARED
        Utilities/LogicFileHandler.cpp Utilities/LogicFileHandler.h
        Utilities/ConfigurationHandler.cpp Utilities/ConfigurationHandler.h
        Utilities/HandConfiguration.h
        Player/Player.cpp Player/Player.h)

add_library(GameEntities SHARED
        GameEntities/HandProcess.cpp GameEntities/HandProcess.h
        GameEntities/Card.cpp GameEntities/Card.h
        GameEntities/TableBoard.h
        GameEntities/Deck.cpp GameEntities/Deck.h
        Utilities/SafeQueue.h)

add_library(Player SHARED Player/Player.cpp Player/Player.h
        Player/PlayerInterface.h
        Player/Profile.h)

target_link_libraries(GameEntities Logger ConfigurationHandler)

# enable_testing()
# find_package(GTest REQUIRED)

# include_directories(${GTEST_INCLUDE_DIRS})
# message(${GTEST_LIBRARIES})
# message(${GTEST_INCLUDE_DIRS})

# add_executable(tests
#         tests/main.cpp
#         tests/GameEntities.cpp
#         tests/Player.cpp
#         tests/Utilities.cpp
#         )

# target_link_libraries(tests ${GTEST_LIBRARIES} pthread Logger GameEntities Player ConfigurationHandler)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

add_library(logic_lib INTERFACE)
target_link_libraries(logic_lib INTERFACE spdlog::spdlog Logger GameEntities Player ConfigurationHandler)