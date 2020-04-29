#include "generic_lock.h"
#include "thread_pool.h"
#include "gtest/gtest.h"

#include <stack>

std::stack<int32_t> s;

static void task() {
    for (int i = 0; i < 100000; i++) {
        s.push(0);
        s.pop();
    }
}

TEST(GenericLock, Mutex) {
    {
        ThreadPool pool;
        pool.Start(8);
        for (int i = 0; i < 8; i++) { pool.Run(task); }
        ::sleep(8);
        pool.Stop();
    }
    EXPECT_EQ(s.empty(), true);
}

TEST(GenericLock, SpinByTAS) {
    {
        for (int i = 0; i < 8; i++) {

        }
    }
}

TEST(GenericLock, SpinByCAS) {
    {
        for (int i = 0; i < 8; i++) {
            
        }
    }
}
