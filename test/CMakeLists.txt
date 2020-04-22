
set(TESTS
    #filelock_test
    #strtok_test
    #jsonreflect_test
    #rediswrapper_test
    timewrapper_test
    mysqlwrapper_test
    httpserver_test
    logstream_test
    logging_test
    lrucache_test
    )
foreach (test ${TESTS})
    add_executable(${test} ./${test}/${test}.cc)
    target_link_libraries(${test} common)
    target_link_libraries(${test} ${GTEST_LIBRARY})
    target_link_libraries(${test} ${GTEST_MAIN_LIBRARY})
    #target_link_libraries(${test} ${HIREDIS_LIBRARY})
endforeach()


