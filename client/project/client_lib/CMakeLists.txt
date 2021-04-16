find_package(Boost 1.65.1 REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -lboost_thread -lboost_system -pthread")

set(CLIENT_SRC src/client.cpp)

add_library(client_lib ${CLIENT_SRC})

target_link_libraries(client_lib ${Boost_LIBRARIES})

add_subdirectory(tests)
