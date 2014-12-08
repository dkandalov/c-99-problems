#include <string>
#include "p99-tree.cpp"
#include "lib/gtest-1.7.0/include/gtest/gtest.h"

void expectEqualTrees(List<Tree<char> *> expected, List<Tree<char> *> actual) {
    EXPECT_EQ(expected.size(), actual.size());
    for (auto i = expected.begin(), j = actual.begin(); i != expected.end(); i++, j++) {
        std::cout << "expected tree: " << (*i)->toString() << "\n";
        std::cout << "actual tree:   " << (*j)->toString() << "\n";
        std::flush(std::cout);
        EXPECT_EQ(**i, *j);
    }
}

void expectZeroTreeCounter() {
    EXPECT_EQ(0, treeNodesCounter());
}


TEST(P5X, ConstructAndPrintTree) {
    Tree<char>* tree =
        node('a',
            node('b',
                    node('d'),
                    node('e')),
            node('c',
                    emptyNode<char>(),
                    node('f',
                            node('g'),
                            emptyNode<char>()))
        );
    EXPECT_EQ(
            "T(a T(b T(d . .) T(e . .)) T(c . T(f T(g . .) .)))",
            tree->toString()
    );
    EXPECT_EQ(7, tree->size());

    delete(tree);
    expectZeroTreeCounter();
}

TEST(P5X, TreeEquality) {
    auto nodeA = node('a');
    auto nodeB = node('b');
    auto empty = emptyNode<char>();

    EXPECT_TRUE((*empty) == empty);
    EXPECT_TRUE((*nodeA) == nodeA);
    EXPECT_FALSE((*nodeA) == empty);
    EXPECT_FALSE((*nodeA) == nodeB);

    delete(nodeA);
    delete(nodeB);
    delete(empty);

    Tree<char>* tree1 =
        node('a',
            node('b',
                    node('d'),
                    node('e')),
            emptyNode<char>()
        );
    Tree<char>* tree2 =
        node('a',
            node('b',
                    node('d'),
                    node('e')),
            emptyNode<char>()
        );

    EXPECT_EQ(*tree1, tree2);

    delete(tree1);
    delete(tree2);
    expectZeroTreeCounter();
}

TEST(P55_, AddAllPossibleLeafsToATree) {
    List<Tree<char>*> expected = {
            node('x',
                node('x'),
                emptyNode<char>()
            ),
            node('x',
                emptyNode<char>(),
                node('x')
            )
    };
    Tree<char>* rootNode = node('x');
    List<Tree<char>*> actual = addAllPossibleLeafs(rootNode, 'x');

    expectEqualTrees(expected, actual);

    delete(rootNode);
    deleteAll(expected);
    deleteAll(actual);
    expectZeroTreeCounter();
}

TEST(P55, ConstructCompletelyBalancedTree) {
    List<Tree<char>*> expected = {
            node('x',
                node('x', node('x'), emptyNode<char>()),
                node('x')
            ),
            node('x',
                node('x', emptyNode<char>(), node('x')),
                node('x')
            ),
            node('x',
                node('x'),
                node('x', node('x'), emptyNode<char>())
            ),
            node('x',
                node('x'),
                node('x', emptyNode<char>(), node('x'))
            )
    };
    List<Tree<char>*> actual = constructBalancedTrees(4, 'x');
    List<Tree<char>*> actual2 = constructBalancedTrees2(4, 'x');

    expectEqualTrees(expected, actual);
    expectEqualTrees(expected, actual2);

    deleteAll(expected);
    deleteAll(actual);
    deleteAll(actual2);
    expectZeroTreeCounter();
}

TEST(P56, ConstructOfSymmetricTree) {
    auto tree = node('a');
    EXPECT_TRUE(tree->isSymmetric());
    delete(tree);

    tree = node('a', node('b'), emptyNode<char>());
    EXPECT_FALSE(tree->isSymmetric());
    delete(tree);

    tree = node('a', node('b'), node('c'));
    EXPECT_TRUE(tree->isSymmetric());
    delete(tree);

    tree = node('a',
            node('b', emptyNode<char>(), node('e')),
            node('c', node('f'), emptyNode<char>())
    );
    EXPECT_TRUE(tree->isSymmetric());
    delete(tree);

    tree = node('a',
            node('b', node('d'), emptyNode<char>()),
            node('c', node('f'), emptyNode<char>())
    );
    EXPECT_FALSE(tree->isSymmetric());
    delete(tree);

    expectZeroTreeCounter();
}