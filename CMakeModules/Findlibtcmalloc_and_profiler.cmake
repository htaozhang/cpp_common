
MESSAGE(STATUS "Using bundled Findlibtcmalloc_and_profiler.cmake...")

FIND_LIBRARY(
    TCMALLOC_AND_PROFILER_LIBRARY 
    NAMES tcmalloc_and_profiler
    PATHS ${CMAKE_CURRENT_LIST_DIR}/../thirdparty/gperftools/lib/ NO_DEFAULT_PATH
   )

