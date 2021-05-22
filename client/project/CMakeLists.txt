find_package(Boost 1.71.0 REQUIRED COMPONENTS system log thread)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -lboost_thread -lboost_system -pthread")

add_subdirectory(msgfabric_lib)
include_directories(msgfabric_lib/include)

add_subdirectory(client_lib)
include_directories(client_lib/include)

add_subdirectory(ui_lib)

include_directories(ui_lib)
include_directories(ui_lib/Base)
include_directories(ui_lib/Fragments)
include_directories(ui_lib/GameElements)

add_executable(client_target src/main.cpp)

target_link_libraries(client_target
                      ui_lib
                      msgfabric_lib 
                      client_lib
                      ${Boost_LIBRARIES}
                     )
