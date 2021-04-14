cmake_minimum_required(VERSION 3.5)

project(UiTest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/Base")
include_directories("${PROJECT_SOURCE_DIR}/Fragments")
include_directories("${PROJECT_SOURCE_DIR}/Game_Elements")
include_directories("${PROJECT_SOURCE_DIR}/Game_Elements/Gameplay_buttons")
include_directories("${PROJECT_SOURCE_DIR}/Ui_elements")

set(PROJECT_SOURCES
        Base/basefragment.cpp
        Base/basefragment.h
        Base/basescreensfactory.cpp
        Base/basescreensfactory.h
        Base/navigator.h
        Base/navigator.cpp
        Fragments/loginfragment.cpp
        Fragments/loginfragment.h
        Fragments/registrationfragment.cpp
        Fragments/registrationfragment.h
        Fragments/menufragment.cpp
        Fragments/menufragment.h
        Fragments/settingsfragment.cpp
        Fragments/settingsfragment.h
        Fragments/gamesearchfragment.cpp
        Fragments/gamesearchfragment.h
        Fragments/gamefragment.cpp
        Fragments/gamefragment.h
        Game_Elements/otherplayer.cpp
        Game_Elements/otherplayer.h
        Game_Elements/player.cpp
        Game_Elements/player.h
        screenfactory.cpp
        screenfactory.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(UiTest
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(UiTest SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(UiTest
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(UiTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
