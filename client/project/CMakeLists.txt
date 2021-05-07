add_subdirectory(msgfabric_lib)

include_directories(msgfabric_lib/include)

add_subdirectory(client_lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(client_lib/include)

add_executable(client_target src/main.cpp)

target_link_libraries(client_target 
                      msgfabric_lib 
                      client_lib
                     )