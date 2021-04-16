add_subdirectory(client_lib)

add_executable(client_target src/main.cpp)

target_link_libraries(client_target client_lib)