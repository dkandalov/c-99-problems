#include "p99.cpp"
#include "lib/gtest-1.7.0/include/gtest/gtest.h"

TEST(P1, LastElementOfList) {
    EXPECT_EQ(8, lastElementOf((std::list<int>) {1, 1, 2, 3, 5, 8}));
}

TEST(P2, PenultimateElementOfList) {
    EXPECT_THROW(penultimate((std::list<int>) {}), std::runtime_error);
    EXPECT_THROW(penultimate((std::list<int>) {1}), std::runtime_error);

    EXPECT_EQ(1, penultimate((std::list<int>) {1, 2}));
    EXPECT_EQ(5, penultimate((std::list<int>) {1, 1, 2, 3, 5, 8}));
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
    EXPECT_EQ(0, sizeOf((std::list<int>) {}));
    EXPECT_EQ(1, sizeOf((std::list<int>) {1}));
    EXPECT_EQ(6, sizeOf((std::list<int>) {1, 1, 2, 3, 5, 8}));
}

TEST(P5, ReverseList) {
    std::list<int> expected = {};
    EXPECT_EQ(expected, reverse((std::list<int>) {}));

    expected = {1};
    EXPECT_EQ(expected, reverse((std::list<int>) {1}));

    expected = {8, 5, 3, 2, 1, 1};
    EXPECT_EQ(expected, reverse((std::list<int>) {1, 1, 2, 3, 5, 8}));
}

TEST(P6, IsListAPalindrome) {
    EXPECT_EQ(true, isPalindrome((std::list<int>) {}));
    EXPECT_EQ(true, isPalindrome((std::list<int>) {1}));
    EXPECT_EQ(true, isPalindrome((std::list<int>) {1, 2, 3, 2, 1}));
    EXPECT_EQ(false, isPalindrome((std::list<int>) {1, 1, 2, 3, 5, 8}));
}
