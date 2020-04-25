#include "blocking_queue.h"
#include "bounded_blocking_queue.h"
#include "gtest/gtest.h"

TEST(BlockingQueue, Test1) {
    BlockingQueue<int> queue;
    EXPECT_EQ(queue.Size(), 0);
    queue.Enqueue(1);
    queue.Enqueue(2);
    EXPECT_EQ(queue.Size(), 2);

    EXPECT_EQ(queue.Dequeue(), 1);
    EXPECT_EQ(queue.Dequeue(), 2);
    EXPECT_EQ(queue.Size(), 0);
}

TEST(BoundedBlockingQueue, Test1) {
}
