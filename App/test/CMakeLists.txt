set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage --coverage -pthread")
set(CMAKE_CXX_STANDARD 17)


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



file(GLOB tests "${PROJECT_SOURCE_DIR}/test/*.cpp")
list(REMOVE_ITEM tests "${PROJECT_SOURCE_DIR}/test/main.cpp")

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

)


foreach(file ${tests})
  set(name)
  get_filename_component(name ${file} NAME_WE)
  add_executable("${name}_tests"
    ${file}
    "${PROJECT_SOURCE_DIR}/test/main.cpp"
    )
  target_link_libraries("${name}_tests" MyLib gtest_main)
  add_test(NAME ${name} COMMAND "${name}_tests")
endforeach()
