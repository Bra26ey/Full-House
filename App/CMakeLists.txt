cmake_minimum_required(VERSION 3.5)

project(UiTest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "--coverage -Wall")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()
add_subdirectory(test)

find_package(Qt5Widgets)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/")
include_directories("${PROJECT_SOURCE_DIR}/Base")
include_directories("${PROJECT_SOURCE_DIR}/Fragments")
include_directories("${PROJECT_SOURCE_DIR}/Game_Elements")
include_directories("${PROJECT_SOURCE_DIR}/Game_Elements/Gameplay_Buttons")

include_directories("${PROJECT_SOURCE_DIR}/Ui_elements/For_Login")
include_directories("${PROJECT_SOURCE_DIR}/Ui_elements/For_Registration")
include_directories("${PROJECT_SOURCE_DIR}/Ui_elements/For_Menu")
include_directories("${PROJECT_SOURCE_DIR}/Ui_elements/For_Settings")
include_directories("${PROJECT_SOURCE_DIR}/Ui_elements/For_Search")

set(PROJECT_SOURCES
        ${PROJECT_SOURCE_DIR}/Base/basefragment.cpp
        ${PROJECT_SOURCE_DIR}/Base/basescreensfactory.cpp
        ${PROJECT_SOURCE_DIR}/Base/navigator.cpp

        ${PROJECT_SOURCE_DIR}/Fragments/loginfragment.cpp
        ${PROJECT_SOURCE_DIR}/Fragments/registrationfragment.cpp
        ${PROJECT_SOURCE_DIR}/Fragments/menufragment.cpp
        ${PROJECT_SOURCE_DIR}/Fragments/settingsfragment.cpp
        ${PROJECT_SOURCE_DIR}/Fragments/gamesearchfragment.cpp
        ${PROJECT_SOURCE_DIR}/Fragments/gamefragment.cpp

        ${PROJECT_SOURCE_DIR}/Game_Elements/otherplayer.cpp
        ${PROJECT_SOURCE_DIR}/Game_Elements/player.cpp
        ${PROJECT_SOURCE_DIR}/Game_Elements/card.cpp
        ${PROJECT_SOURCE_DIR}/Game_Elements/chips.cpp
        ${PROJECT_SOURCE_DIR}/Game_Elements/playtable.cpp
        ${PROJECT_SOURCE_DIR}/Game_Elements/playerinfolabel.cpp

        ${PROJECT_SOURCE_DIR}/Game_Elements/Gameplay_Buttons/betbutton.cpp
        ${PROJECT_SOURCE_DIR}/Game_Elements/Gameplay_Buttons/checkbutton.cpp
        ${PROJECT_SOURCE_DIR}/Game_Elements/Gameplay_Buttons/foldbutton.cpp
        ${PROJECT_SOURCE_DIR}/Game_Elements/Gameplay_Buttons/raisebutton.cpp
        ${PROJECT_SOURCE_DIR}/Game_Elements/Gameplay_Buttons/betslider.cpp

        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Login/gotoregistrationbutton.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Login/loginbutton.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Login/logintextbox.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Login/passwordtextbox.cpp

        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Registration/emailtextbox.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Registration/registrationbutton.cpp

        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Menu/gotologinbutton.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Menu/gotosearchbutton.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Menu/gotosettingsbutton.cpp

        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Settings/mutebutton.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Settings/playerinfo.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Settings/volumeslider.cpp

        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Search/connecttoroombutton.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Search/gotomenubutton.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Search/roomidtextbox.cpp
        ${PROJECT_SOURCE_DIR}/Ui_elements/For_Search/roompasswordtextbox.cpp

        ${PROJECT_SOURCE_DIR}/screenfactory.cpp
        ${PROJECT_SOURCE_DIR}/mainwindow.cpp

        ${PROJECT_SOURCE_DIR}/main.cpp
)

add_library(MyLib SHARED ${PROJECT_SOURCES})
target_link_libraries(MyLib Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(UiTest)
else()
    add_executable(UiTest)
endif()

target_link_libraries(UiTest PRIVATE MyLib)
