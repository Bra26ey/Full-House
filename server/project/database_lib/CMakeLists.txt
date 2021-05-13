set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -fprofile-arcs -ftest-coverage")

include_directories(/usr/include/cppconn)
link_directories(/usr/lib /usr/local/lib)

include_directories(Connection)
include_directories(Models)
include_directories(FileHandler)
include_directories(include)

file(GLOB CONNECTION_SRC "Connection/*.cpp")
file(GLOB FILEHANDLER_SRC "FileHandler/*.cpp")
file(GLOB MODELS_SRC "Models/*.cpp")

set(DATABASE_SRC ${CONNECTION_SRC} ${FILEHANDLER_SRC} ${MODELS_SRC})

add_library(database_lib SHARED ${DATABASE_SRC})

target_link_libraries(database_lib pthread mysqlcppconn)

enable_testing()
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})
# add_executable(tests tests/main.cpp tests/test01_FileHandler.cpp tests/test02_Connection.cpp tests/test03_User.cpp tests/test04_Board.cpp ${DATABASE_SRC} tests/test05_Extra.cpp)
# target_link_libraries(tests ${GTEST_LIBRARIES} pthread mysqlcppconn database_lib)
