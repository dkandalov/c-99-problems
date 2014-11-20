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
            "T(97 T(98 T(100 . .) T(101 . .)) T(99 . T(102 T(103 . .) .)))",
            tree->toString()
    );
}

