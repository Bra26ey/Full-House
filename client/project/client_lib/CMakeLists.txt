find_package(Boost 1.65.1 REQUIRED COMPONENTS system log thread)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include_directories(../msgfabric_lib/include)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -lboost_thread -lboost_system -pthread")

file(GLOB CLIENT_LIB_SRC "src/*.cpp")

add_library(client_lib SHARED ${CLIENT_LIB_SRC})

target_link_libraries(client_lib
                      msgfabric_lib
                      ${Boost_LIBRARIES})

# add_subdirectory(tests)
