#include "generic_thread.h"
#include "gtest/gtest.h"



TEST(GenericThread, _thread_name) {
    set_thread_name("hello");
    EXPECT_STREQ(get_thread_name(), "hello");
}

