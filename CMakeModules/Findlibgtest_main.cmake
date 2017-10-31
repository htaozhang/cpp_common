
MESSAGE(STATUS "Using bundled Findlibgtest_main.cmake...")

FIND_LIBRARY(
    GTEST_MAIN_LIBRARY 
    NAMES gtest_main
    PATHS ${CMAKE_CURRENT_LIST_DIR}/../thirdparty/gtest/lib/
   )

