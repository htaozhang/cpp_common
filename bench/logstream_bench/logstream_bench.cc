
#include "log_stream.h"
#include "time_wrapper.h"

#include <stdio.h>

#include <sstream>

const size_t SIZE = 1000000;

template<typename T>
void BenchPrintf(const char* fmt) {
    TimeWrapper start(TimeWrapper::Now());
    char buf[32];
    for (size_t i = 0; i < SIZE; i++) {
        snprintf(buf, sizeof(buf), fmt, (T)(i));
    }
    TimeWrapper end(TimeWrapper::Now());
   
    printf("BenchPrintf: %fsec\n", end - start);
}

template<typename T>
void BenchStringStream() {
    TimeWrapper start(TimeWrapper::Now());
    std::ostringstream oss;
    for (size_t i = 0; i < SIZE; i++) {
        oss << (T)(i);
        oss.seekp(0, std::ios_base::beg);
    }
    TimeWrapper end(TimeWrapper::Now());
    
    printf("BenchStringStream: %fsec\n", end - start);
}

template<typename T>
void BenchLogStream() {
    TimeWrapper start(TimeWrapper::Now());
    LogStream ls;
    for (size_t i = 0; i < SIZE; i++) {
        ls << (T)(i);
        ls.Reset();
    }
    TimeWrapper end(TimeWrapper::Now());
    
    printf("BenchLogStream: %fsec\n", end - start);
}


int main() {
    puts("\nint");
    BenchPrintf<int>("%d");
    BenchStringStream<int>();
    BenchLogStream<int>();

    puts("\ndouble");
    BenchPrintf<double>("%.12g");
    BenchStringStream<double>();
    BenchLogStream<double>();

    puts("\nint64_t");
    BenchPrintf<int64_t>("%" PRId64);
    BenchStringStream<int64_t>();
    BenchLogStream<int64_t>();

    puts("\nvoid*");
    BenchPrintf<void*>("%p");
    BenchStringStream<void*>();
    BenchLogStream<void*>();
    return 0;
}
