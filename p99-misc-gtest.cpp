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

TEST(P92, VonKochConjecture) {
    using namespace VonKochConjecture;

    vector<Link<char>> treeLinks = {
            Link<char>('a', 'b'), Link<char>('b', 'c')
    };
    vector<p<Solution>> solutions = labelTree(treeLinks);

    EXPECT_EQ(4, solutions.size());
    EXPECT_EQ("(2->1 1->3)", solutions[0]->toString());
    EXPECT_EQ("(1->3 3->2)", solutions[1]->toString());
    EXPECT_EQ("(3->1 1->2)", solutions[2]->toString());
    EXPECT_EQ("(2->3 3->1)", solutions[3]->toString());
    std::cout << "Solutions: " << "\n";
    for (auto& item : solutions) std::cout << item->toString() << "\n";


    treeLinks = {
            Link<char>('a', 'd'), Link<char>('a', 'g'), Link<char>('a', 'b'),
            Link<char>('b', 'e'), Link<char>('b', 'c'), Link<char>('e', 'f')
    };
    solutions = labelTree(treeLinks);

    EXPECT_EQ(52, solutions.size());
    EXPECT_EQ("(2->3 2->5 2->6 6->1 6->4 1->7)", solutions[0]->toString());
    EXPECT_EQ("(2->5 2->3 2->6 6->1 6->4 1->7)", solutions[1]->toString());
//    std::cout << "Solutions: " << "\n";
//    for (auto& item : solutions) std::cout << item.toString() << "\n";
}

TEST(P92, VonKochConjecture_ComplexExample) {
    using namespace VonKochConjecture;

    vector<Link<char>> treeLinks = {
            Link<char>('a', 'b'), Link<char>('a', 'h'), Link<char>('a', 'i'),
            Link<char>('a', 'g'), Link<char>('a', 'c'), Link<char>('c', 'f'),
            Link<char>('c', 'd'), Link<char>('d', 'k'), Link<char>('c', 'e'),
            Link<char>('e', 'q'), Link<char>('q', 'm'), Link<char>('q', 'n'),
            Link<char>('n', 'p')
    };
    // TODO too slow
    vector<p<Solution>> solutions = labelTree(treeLinks);

    EXPECT_EQ(4, solutions.size());
    EXPECT_EQ("(2->1 1->3)", solutions[0]->toString());
    std::cout << "Solutions: " << "\n";
//    for (auto& item : solutions) std::cout << item->toString() << "\n";
}
