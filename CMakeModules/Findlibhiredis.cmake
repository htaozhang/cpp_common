
MESSAGE(STATUS "Using bundled Findlibhiredis.cmake...")

FIND_LIBRARY(
    HIREDIS_LIBRARY 
    NAMES hiredis
    PATHS ${CMAKE_CURRENT_LIST_DIR}/../thirdparty/hiredis/lib/
   )

