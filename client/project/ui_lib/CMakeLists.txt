set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Multimedia REQUIRED)

include_directories(Base)
include_directories(Fragments)
include_directories(GameElements)

include_directories(../client_lib/include)
include_directories(../msgfabric_lib/include)


file(GLOB PROJECT_SOURCES "Base/*.cpp" "Fragments/*.cpp" "GameElements/*.cpp" "UiElements/*.cpp" "*.cpp" "GameElements/Gameplay_Buttons/*.cpp" "*.qrc")

add_library(ui_lib SHARED ${PROJECT_SOURCES} )
target_link_libraries(ui_lib
                      Qt${QT_VERSION_MAJOR}::Widgets
                      Qt${QT_VERSION_MAJOR}::Multimedia
                      )

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(FullHouse)
# else()
#     add_executable(FullHouse)
# endif()

# target_link_libraries(FullHouse ui_lib)

#cmake . && cmake --build . && cmake --build . --target package
#install(TARGETS FullHouse ui_lib
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib)

#install(TARGETS FullHouse DESTINATION /home/poker)



## Задать некоторые характеристики пакета:
#set(CPACK_PACKAGE_NAME "FullHouse")
#set(CPACK_PACKAGE_VENDOR "4 retard production")
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "alexander.klonov@mail.ru")
#set(CPACK_PACKAGE_DESCRIPTION "Made by Techopark students")


## Задать используемый генератор пакетов:
#set(CPACK_GENERATOR "DEB")

## Включить стандартный модуль "CPack":
#include(CPack)
