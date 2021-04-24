include_directories(server_lib/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_thread -lboost_system -pthread")

add_executable(server_target src/main.cpp)

add_subdirectory(server_lib)

target_link_libraries(server_target server_lib)