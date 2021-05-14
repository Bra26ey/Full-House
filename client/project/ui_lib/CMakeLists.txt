cmake_minimum_required(VERSION 3.5)

project(FullHouse LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "--coverage -Wall")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.65.1 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})


find_package(Qt5Widgets)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Multimedia REQUIRED)

include_directories(Base)
include_directories(Fragments)
include_directories(GameElements)
include_directories(GameStatus)
include_directories(GameElements/Gameplay_Buttons)

include_directories(UiElements/For_Login)
include_directories(UiElements/For_Registration)
include_directories(UiElements/For_Menu)
include_directories(UiElements/For_Settings)
include_directories(UiElements/For_Search)

include_directories(../client_lib/include)
include_directories(../msgfabric_lib/include)

set(PROJECT_SOURCES
        Base/basefragment.cpp
        Base/basescreensfactory.cpp
        Base/navigator.cpp

        GameStatus/gamestatus.cpp

        Fragments/loginfragment.cpp
        Fragments/registrationfragment.cpp
        Fragments/menufragment.cpp
        Fragments/settingsfragment.cpp
        Fragments/gamesearchfragment.cpp
        Fragments/gamefragment.cpp
        Fragments/rulesfragment.cpp
        Fragments/creditmenufragment.cpp
        Fragments/equationsfragment.cpp

        GameElements/otherplayer.cpp
        GameElements/player.cpp
        GameElements/card.cpp
        GameElements/chips.cpp
        GameElements/playtable.cpp
        GameElements/playerinfolabel.cpp
        GameElements/dealerlogo.cpp
        GameElements/turnspark.cpp
        GameElements/winlabel.cpp

        GameElements/Gameplay_Buttons/betbutton.cpp
        GameElements/Gameplay_Buttons/checkbutton.cpp
        GameElements/Gameplay_Buttons/foldbutton.cpp
        GameElements/Gameplay_Buttons/raisebutton.cpp
        GameElements/Gameplay_Buttons/betslider.cpp

        UiElements/For_Login/gotoregistrationbutton.cpp
        UiElements/For_Login/loginbutton.cpp
        UiElements/For_Login/logintextbox.cpp
        UiElements/For_Login/passwordtextbox.cpp

        UiElements/For_Registration/emailtextbox.cpp
        UiElements/For_Registration/registrationbutton.cpp

        UiElements/For_Menu/gotologinbutton.cpp
        UiElements/For_Menu/gotosearchbutton.cpp
        UiElements/For_Menu/gotosettingsbutton.cpp

        UiElements/For_Settings/mutebutton.cpp
        UiElements/For_Settings/playerinfo.cpp
        UiElements/For_Settings/volumeslider.cpp

        UiElements/For_Search/connecttoroombutton.cpp
        UiElements/For_Search/gotomenubutton.cpp
        UiElements/For_Search/roomidtextbox.cpp
        UiElements/For_Search/roompasswordtextbox.cpp

        screenfactory.cpp
        mainwindow.cpp

        resource.qrc
)

add_library(ui_lib SHARED ${PROJECT_SOURCES})
target_link_libraries(ui_lib
                      Qt${QT_VERSION_MAJOR}::Widgets
                      Qt${QT_VERSION_MAJOR}::Multimedia
                      ${Boost_LIBRARIES}
                      client_lib)

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
