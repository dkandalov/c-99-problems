#include <gtest/gtest.h>
#include "p99-misc.cpp"

TEST(P90, EightQueensProblem) {
    EXPECT_EQ(1, solveEightQueensProblem(1).size());
    EXPECT_EQ(0, solveEightQueensProblem(2).size());
    EXPECT_EQ(0, solveEightQueensProblem(3).size());
    EXPECT_EQ(2, solveEightQueensProblem(4).size());
    EXPECT_EQ(10, solveEightQueensProblem(5).size());
    EXPECT_EQ(92, solveEightQueensProblem(8).size());
}
