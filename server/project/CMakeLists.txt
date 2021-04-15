find_package(Boost 1.65.1 REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include_directories(include)

file(GLOB SERVER_SRC "src/*.cpp")

add_library(server_lib SHARED ${SERVER_SRC})

add_executable(server_target src/main.c)
target_link_libraries(server_target ${Boost_LIBRARIES} server_lib)

add_subdirectory(tests)
