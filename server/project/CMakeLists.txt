add_subdirectory(msgfabric_lib)

include_directories(msgfabric_lib/include)

add_subdirectory(server_lib)

include_directories(server_lib/include)

add_executable(server_target src/main.cpp)

target_link_libraries(server_target server_lib)
