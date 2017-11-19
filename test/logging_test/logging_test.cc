#include "logging.h"
#include "gtest/gtest.h"


TEST(Logging, All) {
    Logging::SetLevel(Logging::Level::L_TRACE);
    
    LOG_TRACE << "hello, trace";
    LOG_DEBUG << "hello, debug";
    LOG_INFO << "hello, info";
    LOG_WARN << "hello, warn";
    LOG_ERROR << "hello, error";
    LOG_SYSERR << "hello, syserr";
    //LOG_FATAL << "hello, fatal";
}
