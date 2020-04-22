#include "logging.h"
#include "gtest/gtest.h"


// TEST(Logging, Color) {
// #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
// #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
// #endif
//     HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
//     if (out != INVALID_HANDLE_VALUE) {
//         DWORD mode = 0;
//         //EXPECT_TRUE(GetConsoleMode(out, &mode) != 0);
//         mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
//         std::cout << mode << std::endl;
//         //EXPECT_TRUE(SetConsoleMode(out, mode) != 0);
//     }
// }

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
