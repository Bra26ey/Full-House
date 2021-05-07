add_subdirectory(msgfabric_lib)
include_directories(msgfabric_lib/include)

add_subdirectory(logic_lib)
include_directories(logic_lib/Player)
include_directories(logic_lib/Utilities)
include_directories(logic_lib/GameEntities)

add_subdirectory(server_lib)
include_directories(server_lib/include)

add_executable(server_target src/main.cpp)

target_link_libraries(server_target server_lib)
