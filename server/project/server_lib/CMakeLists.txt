find_package(Boost 1.65.1 REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -lboost_thread -lboost_system -pthread")

file(GLOB SERVER_LIB_SRC "src/*.cpp")

add_library(server_lib SHARED ${SERVER_LIB_SRC})

add_subdirectory(tests)