find_package(Boost 1.65.1 REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include_directories(include)

set(CLIENT_SRC src/client.cpp)

add_executable(client_target CLIENT_SRC)
target_link_libraries(client_target ${Boost_LIBRARIES})

add_subdirectory(tests)
