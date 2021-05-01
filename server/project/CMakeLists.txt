add_subdirectory(server_lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(server_lib/include)

add_executable(server_target src/main.cpp)

target_link_libraries(server_target server_lib)
