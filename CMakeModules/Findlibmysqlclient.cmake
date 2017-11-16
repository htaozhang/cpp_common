
MESSAGE(STATUS "Using bundled Findlibmysqlclient.cmake...")

FIND_LIBRARY(
    MYSQLCLIENT_LIBRARY 
    NAMES mysqlclient
    PATHS /usr/lib64 /usr/local/lib
   )

