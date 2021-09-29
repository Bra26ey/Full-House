include_directories(../msgfabric_lib/include)
include_directories(../logic_lib/Player)
include_directories(../logic_lib/Utilities)
include_directories(../logic_lib/GameEntities)
include_directories(../database_lib/include)
include_directories(../database_lib/Connection)
include_directories(../database_lib/FileHandler)
include_directories(../database_lib/Models)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK --coverage -lboost_thread -pthread")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_thread -lboost_system -pthread")

file(GLOB SERVER_LIB_SRC "src/*.cpp")

add_library(server_lib SHARED ${SERVER_LIB_SRC})

target_link_libraries(server_lib
                      msgfabric_lib
                      logic_lib
                      database_lib
                     )

# add_subdirectory(tests)
