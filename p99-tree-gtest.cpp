#include <string>
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
    auto nodeA = node<char>('a');
    auto nodeB = node<char>('b');
    auto empty = emptyNode<char>();

    EXPECT_TRUE((*empty) == empty);
    EXPECT_TRUE((*nodeA) == nodeA);
    EXPECT_FALSE((*nodeA) == empty);
    EXPECT_FALSE((*nodeA) == nodeB);

    delete(nodeA);
    delete(nodeB);
    delete(empty);

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

void expectEqualLists(List<Tree<char>*> expected, List<Tree<char>*> actual) {
    EXPECT_EQ(expected.size(), actual.size());
    for (auto i = expected.begin(), j = actual.begin(); i != expected.end(); i++, j++) {
        std::cout << "expected tree: " << (*i)->toString() << "\n";
        std::cout << "actual tree:   " << (*j)->toString() << "\n";
        EXPECT_EQ(**i, *j);
    }
}

TEST(P55_, AddAllPossibleLeafsToATree) {
    List<Tree<char>*> expected = {
            node<char>('x',
                node<char>('x'),
                emptyNode<char>()
            ),
            node<char>('x',
                emptyNode<char>(),
                node<char>('x')
            )
    };
    List<Tree<char>*> actual = addAllPossibleLeafs(node('x'), 'x');

    expectEqualLists(expected, actual);

    for (auto tree : expected) delete(tree);
    for (auto tree : actual) delete(tree);
}

TEST(P55, ConstructCompletelyBalancedTree) {
    List<Tree<char>*> expected = {
            emptyNode<char>()
    };
    List<Tree<char>*> actual = constructBalancedTrees(4, 'x');

    expectEqualLists(expected, actual);

    for (auto tree : expected) delete(tree);
    for (auto tree : actual) delete(tree);
}