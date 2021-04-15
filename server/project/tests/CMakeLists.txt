find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage --coverage -Wall -Wextra -Werror")

file(GLOB TEST_SRC "src/*.cpp")

add_executable(tests ${TEST_SRC})

set_target_properties(tests PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    tests 
    ${GTEST_BOTH_LIBRARIES}
    stdc++fs
    successive_sort parallel_sort
)

add_test(
    NAME SuccesiveTest
    COMMAND tests ${PROJECT_SOURCE_DIR}/project/tests/data --gtest_filter=*Successive*
)

add_test(
    NAME ParallelTest
    COMMAND tests ${PROJECT_SOURCE_DIR}/project/tests/data --gtest_filter=*Parallel*
)
