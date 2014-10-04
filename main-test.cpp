#include "p99.cpp"
#include "lib/gtest-1.7.0/include/gtest/gtest.h"

TEST(P1, LastElementOfList) {
    std::list<int> list = {1, 1, 2, 3, 5, 8};
    EXPECT_EQ(8, lastElementOf(list));
}
