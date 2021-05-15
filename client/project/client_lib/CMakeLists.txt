include_directories(include)
include_directories(../msgfabric_lib/include)

file(GLOB CLIENT_LIB_SRC "src/*.cpp")

add_library(client_lib SHARED ${CLIENT_LIB_SRC})

target_link_libraries(client_lib msgfabric_lib)

# add_subdirectory(tests)
