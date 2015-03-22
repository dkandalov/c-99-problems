#include <gtest/gtest.h>
#include "p99-misc.cpp"

TEST(P90, EightQueensProblem) {
    using namespace EightQueens;
    auto solveAndPrint = [](int boardSize){
        auto solutions = solveEightQueensProblem(boardSize);
        for (auto solution : solutions) {
            std::cout << asString(solution) << "\n";
        }
        return solutions;
    };

    EXPECT_EQ(1, solveAndPrint(1).size());
    EXPECT_EQ(0, solveAndPrint(2).size());
    EXPECT_EQ(0, solveAndPrint(3).size());
    EXPECT_EQ(2, solveAndPrint(4).size());
    EXPECT_EQ(10, solveAndPrint(5).size());
    EXPECT_EQ(92, solveEightQueensProblem(8).size());
}

TEST(P91, KnightsTour) {
    using namespace KnightsTour;
    EXPECT_EQ(1, findKnightsPath(1).size());
}