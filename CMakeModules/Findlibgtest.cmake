
MESSAGE(STATUS "Using bundled Findlibgtest.cmake...")

FIND_LIBRARY(
    GTEST_LIBRARY 
    NAMES gtest
    PATHS ${CMAKE_CURRENT_LIST_DIR}/../thirdparty/gtest/lib/
   )

