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
}

