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
    EXPECT_EQ(0, findKnightsPath(2).size());
    EXPECT_EQ(0, findKnightsPath(3).size());
    EXPECT_EQ(0, findKnightsPath(4).size());
//    EXPECT_EQ(304, findKnightsPath(5).size());
//    EXPECT_EQ(0, findKnightsPath(8).size());
}

TEST(P91, KnightsTour_Lazy) {
    using namespace KnightsTour;

    auto knightsPath = KnightPathLazy(1);
    EXPECT_EQ(1, knightsPath.nextPath().size());
    EXPECT_EQ(0, knightsPath.nextPath().size());

    knightsPath = KnightPathLazy(2);
    EXPECT_EQ(0, knightsPath.nextPath().size());

    // commented out because it takes ~6.5 sec
//    knightsPath = KnightPathLazy(5);
//    for (int i = 0; i < 304; i++) {
//        EXPECT_EQ(25, knightsPath.nextPath().size());
//        std::cout << i << "\n";
//    }
//    EXPECT_EQ(0, knightsPath.nextPath().size());
}