find_package(Boost 1.65.1 REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include_directories(include)

set(SERVER_SRC src/server.cpp)

add_executable(server_programm src/main.c)
target_link_libraries(server_programm ${Boost_LIBRARIES})

add_subdirectory(tests)
