#include "p99-tree.cpp"
#include "lib/gtest-1.7.0/include/gtest/gtest.h"

TEST(P5X, ConstructAndPrintTree) {
    Tree<char>* tree =
        node<char>('a',
            node<char>('b',
                    node<char>('d'),
                    node<char>('e')),
            node<char>('c',
                    emptyNode<char>(),
                    node<char>('f',
                            node<char>('g'),
                            emptyNode<char>()))
        );
    EXPECT_EQ(
            "T(a T(b T(d . .) T(e . .)) T(c . T(f T(g . .) .)))",
            tree->toString()
    );
    delete(tree);
}

TEST(P5X, TreeEquality) {
    Tree<char>* tree1 =
        node<char>('a',
            node<char>('b',
                    node<char>('d'),
                    node<char>('e')),
            emptyNode<char>()
        );
    Tree<char>* tree2 =
        node<char>('a',
            node<char>('b',
                    node<char>('d'),
                    node<char>('e')),
            emptyNode<char>()
        );
    EXPECT_TRUE((*tree1) == tree2);
    delete(tree1);
    delete(tree2);
}

//TEST(P55, ConstructCompletelyBalancedTree) {
//    List<Tree<char>*> expected = {
//            emptyNode<char>()
//    };
//    EXPECT_EQ(expected, constructBalancedTrees(4, 'x'));
//
//    for (auto tree : expected) {
//        delete(tree);
//    }
//}