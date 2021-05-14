find_package(Boost 1.65.1 REQUIRED COMPONENTS system log thread)
find_package(Threads)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_subdirectory(msgfabric_lib)
include_directories(msgfabric_lib/include)

add_subdirectory(logic_lib)
include_directories(logic_lib/Player)
include_directories(logic_lib/Utilities)
include_directories(logic_lib/GameEntities)

add_subdirectory(database_lib)
include_directories(database_lib/include)
include_directories(database_lib/Connection)
include_directories(database_lib/FileHandler)
include_directories(database_lib/Models)

add_subdirectory(server_lib)
include_directories(server_lib/include)

add_executable(server_target src/main.cpp)

target_link_libraries(server_target
                      server_lib
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${Boost_LIBRARIES})
