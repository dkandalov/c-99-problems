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
    EXPECT_EQ(7, tree->size());

    delete(tree);
}

TEST(P5X, TreeEquality) {
    EXPECT_TRUE((*emptyNode<char>()) == emptyNode<char>());
    EXPECT_TRUE((*node<char>('a')) == node<char>('a'));

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

    EXPECT_EQ(*tree1, tree2);

    delete(tree1);
    delete(tree2);
}

TEST(P55, ConstructCompletelyBalancedTree) {
    List<Tree<char>*> expected = {
            emptyNode<char>()
    };
    List<Tree<char> *> actual = constructBalancedTrees(4, 'x');

    EXPECT_EQ(expected.size(), actual.size());
    // TODO
//    for (auto i = expected.begin, j = actual.begin; ) {
//
//    }

    for (auto tree : expected) delete(tree);
    for (auto tree : actual) delete(tree);

}