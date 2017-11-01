
#aux_source_directory(./ COMMON_SOURCES)

set(COMMON_SOURCES
    #http_server.cc
    log_stream.cc
    #logging.cc
    #mysql_wrapper.cc
    #redis_wrapper.cc
    time_wrapper.cc
    )

ADD_LIBRARY(common STATIC ${COMMON_SOURCES})

SET(libs
    ${GTEST_LIBRARY}
    ${GTEST_MAIN_LIBRARY}
    ${HIREDIS_LIBRARY}
    #${MYSQLCLIENT_LIBRARY}
    ${TCMALLOC_AND_PROFILER_LIBRARY}
    )

foreach (lib ${libs})
    target_link_libraries(common ${lib})
endforeach()

