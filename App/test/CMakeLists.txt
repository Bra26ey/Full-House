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
