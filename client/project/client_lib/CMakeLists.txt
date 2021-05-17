<<<<<<< HEAD
find_package(Boost 1.65.1 REQUIRED COMPONENTS system log thread)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include_directories(../msgfabric_lib/include)

=======
>>>>>>> origin/merge_net_logic
include_directories(include)
include_directories(../msgfabric_lib/include)

file(GLOB CLIENT_LIB_SRC "src/*.cpp")

add_library(client_lib SHARED ${CLIENT_LIB_SRC})

<<<<<<< HEAD
target_link_libraries(client_lib
                      msgfabric_lib
                      ${Boost_LIBRARIES})
=======
target_link_libraries(client_lib msgfabric_lib)
>>>>>>> origin/merge_net_logic

# add_subdirectory(tests)
