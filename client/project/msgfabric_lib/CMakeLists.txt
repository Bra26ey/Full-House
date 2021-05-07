find_package(Boost 1.65.1 REQUIRED COMPONENTS system)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK --coverage")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_thread -lboost_system -pthread")

file(GLOB MSGFABRIC_LIB_SRC "src/*.cpp")

add_library(msgfabric_lib SHARED ${MSGFABRIC_LIB_SRC})

target_link_libraries(msgfabric_lib ${Boost_LIBRARIES})

# add_subdirectory(tests)
