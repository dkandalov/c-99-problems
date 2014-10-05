#include "p99.cpp"
#include "lib/gtest-1.7.0/include/gtest/gtest.h"

TEST(P1, LastElementOfList) {
    EXPECT_EQ(8, lastElementOf({1, 1, 2, 3, 5, 8}));
}

TEST(P2, PenultimateElementOfList) {
    EXPECT_EQ(5, penultimate({1, 1, 2, 3, 5, 8}));
}

TEST(P3, FindNthElementOfList) {
    std::list<int> list = {1, 1, 2, 3, 5, 8};
    EXPECT_EQ(1, getElement(0, list));
    EXPECT_EQ(1, getElement(1, list));
    EXPECT_EQ(2, getElement(2, list));
    EXPECT_EQ(3, getElement(3, list));
    EXPECT_EQ(5, getElement(4, list));
    EXPECT_EQ(8, getElement(5, list));
}

TEST(P4, NumberOfElementsInList) {
    EXPECT_EQ(0, sizeOf({}));
    EXPECT_EQ(1, sizeOf({1}));
    EXPECT_EQ(6, sizeOf({1, 1, 2, 3, 5, 8}));
}