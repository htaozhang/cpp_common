CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")

PROJECT(framework CXX)

INCLUDE_DIRECTORIES(./common
                    ./thirdparty/gtest/include 
                    ./thirdparty/rapidjson/include 
                    ./thirdparty/hiredis/include
                    ./thirdparty/gpreftools/include
                    )

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
FIND_PACKAGE(libgtest MODULE)
FIND_PACKAGE(libgtest_main MODULE)
FIND_PACKAGE(libhiredis MODULE)
FIND_PACKAGE(libmysqlclient MODULE)
FIND_PACKAGE(libtcmalloc_and_profiler MODULE)

message(STATUS "gtest library = " ${GTEST_LIBRARY})
message(STATUS "gtest_main library = " ${GTEST_MAIN_LIBRARY})
message(STATUS "hiredis library = " ${HIREDIS_LIBRARY})
message(STATUS "mysqlclient library = " ${MYSQLCLIENT_LIBRARY})
message(STATUS "tcmalloc_and_profiler library = " ${TCMALLOC_AND_PROFILER_LIBRARY})

SET(CXX_FLAGS
    -I "C:\\MinGW\\include"
    -L "C:\\MinGW\\lib"
    #-march=native
    -D_FILE_OFFSET_BITS=64
    -O2
    -g
    -Wall
    -Wextra
    -Werror
    -Wno-unused-parameter
    -Wconversion
    -Woverloaded-virtual
    #-Wold-style-cast
    -Wshadow
    -std=gnu++11
    )
#IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#    LIST(APPEND CXX_FLAGS -lpthread)
#ENDIF()
STRING(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
MESSAGE(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS})


SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(test)
#ADD_SUBDIRECTORY(bench)


