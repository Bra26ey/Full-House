find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

file(GLOB TEST_SRC "src/*.cpp")

add_executable(tests ${TEST_SRC})

target_link_libraries(
    tests 
    ${GTEST_BOTH_LIBRARIES}
    stdc++fs
    client_lib
)

add_test(
    NAME ClientTest
    COMMAND tests
)
