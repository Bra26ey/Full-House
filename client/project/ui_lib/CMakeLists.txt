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
include_directories(Game_Elements)
include_directories(Game_Elements/Gameplay_Buttons)

include_directories(Ui_elements/For_Login)
include_directories(Ui_elements/For_Registration)
include_directories(Ui_elements/For_Menu)
include_directories(Ui_elements/For_Settings)
include_directories(Ui_elements/For_Search)

include_directories(../client_lib/include)

set(PROJECT_SOURCES
        Base/basefragment.cpp
        Base/basescreensfactory.cpp
        Base/navigator.cpp

        Fragments/loginfragment.cpp
        Fragments/registrationfragment.cpp
        Fragments/menufragment.cpp
        Fragments/settingsfragment.cpp
        Fragments/gamesearchfragment.cpp
        Fragments/gamefragment.cpp
        Fragments/rulesfragment.cpp

        Game_Elements/otherplayer.cpp
        Game_Elements/player.cpp
        Game_Elements/card.cpp
        Game_Elements/chips.cpp
        Game_Elements/playtable.cpp
        Game_Elements/playerinfolabel.cpp
        Game_Elements/dealerlogo.cpp
        Game_Elements/turnspark.cpp
        Game_Elements/winlabel.cpp

        Game_Elements/Gameplay_Buttons/betbutton.cpp
        Game_Elements/Gameplay_Buttons/checkbutton.cpp
        Game_Elements/Gameplay_Buttons/foldbutton.cpp
        Game_Elements/Gameplay_Buttons/raisebutton.cpp
        Game_Elements/Gameplay_Buttons/betslider.cpp

        Ui_elements/For_Login/gotoregistrationbutton.cpp
        Ui_elements/For_Login/loginbutton.cpp
        Ui_elements/For_Login/logintextbox.cpp
        Ui_elements/For_Login/passwordtextbox.cpp

        Ui_elements/For_Registration/emailtextbox.cpp
        Ui_elements/For_Registration/registrationbutton.cpp

        Ui_elements/For_Menu/gotologinbutton.cpp
        Ui_elements/For_Menu/gotosearchbutton.cpp
        Ui_elements/For_Menu/gotosettingsbutton.cpp

        Ui_elements/For_Settings/mutebutton.cpp
        Ui_elements/For_Settings/playerinfo.cpp
        Ui_elements/For_Settings/volumeslider.cpp

        Ui_elements/For_Search/connecttoroombutton.cpp
        Ui_elements/For_Search/gotomenubutton.cpp
        Ui_elements/For_Search/roomidtextbox.cpp
        Ui_elements/For_Search/roompasswordtextbox.cpp

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
