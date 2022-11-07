#include "../src/LRUCache.hpp"
#include <gtest/gtest.h>

TEST(Tests, BigCache) {
    LRUCache<int32_t, int32_t> cache(100);
    cache.put(0, 0);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);

    ASSERT_EQ(*cache.get(0), 0);
    ASSERT_EQ(*cache.get(1), 1);
    ASSERT_EQ(*cache.get(2), 2);
    ASSERT_EQ(*cache.get(3), 3);
}

TEST(Tests, SmallCache) {
    LRUCache<int32_t, int32_t> cache(1);
    cache.put(0, 0);
    ASSERT_EQ(*cache.get(0), 0);

    cache.put(1, 1);
    ASSERT_EQ(cache.get(0), nullptr);
    ASSERT_EQ(*cache.get(1), 1);

    cache.put(2, 2);
    ASSERT_EQ(cache.get(1), nullptr);
    ASSERT_EQ(*cache.get(2), 2);

    cache.put(3, 3);
    ASSERT_EQ(cache.get(2), nullptr);
    ASSERT_EQ(*cache.get(3), 3);
}

TEST(Tests, CheckGetLRU) {
    LRUCache<int32_t, int32_t> cache(2);
    cache.put(0, 0);
    cache.put(1, 1);
    cache.get(0);
    cache.put(2, 2);
    ASSERT_EQ(*cache.get(0), 0);
    ASSERT_EQ(cache.get(1), nullptr);
    ASSERT_EQ(*cache.get(2), 2);
}

TEST(Tests, CheckUpdate) {
    LRUCache<int32_t, int32_t> cache(2);
    cache.put(0, 0);
    cache.put(1, 1);

    cache.put(0, 2);
    cache.put(1, 3);
    ASSERT_EQ(*cache.get(0), 2);
    ASSERT_EQ(*cache.get(1), 3);
}
