set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -fprofile-arcs -ftest-coverage")

include_directories(/usr/include/cppconn)
link_directories(/usr/lib /usr/local/lib)

include_directories(Connection Models FileHandler .)
set(DATABASE_SRC FileHandler/FileHandler.cpp Connection/Connection.cpp Connection/PoolConnections.cpp Connection/SafetyConnection.cpp Models/Board.cpp Models/User.cpp)
add_library(database_lib SHARED ${DATABASE_SRC})
target_link_libraries(database_lib pthread mysqlcppconn)

enable_testing()
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})
# add_executable(tests tests/main.cpp tests/test01_FileHandler.cpp tests/test02_Connection.cpp tests/test03_User.cpp tests/test04_Board.cpp ${DATABASE_SRC} tests/test05_Extra.cpp)
# target_link_libraries(tests ${GTEST_LIBRARIES} pthread mysqlcppconn database_lib)
