
set(BENCHS
    logstream_bench
    )
foreach (bench ${BENCHS})
    add_executable(${bench} ./${bench}/${bench}.cc)
    target_link_libraries(${bench} common)
endforeach()


