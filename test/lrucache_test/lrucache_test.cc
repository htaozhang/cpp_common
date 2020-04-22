#include "lru_cache.h"
#include "gtest/gtest.h"

TEST(LRUCache, Constructor) {
    LRUCache<int, int> lrucache;
    EXPECT_EQ(lrucache.Size(), 0);
    lrucache.Put(100, 0);
    lrucache.Put(101, 2);
    EXPECT_EQ(lrucache.Size(), 2);
}