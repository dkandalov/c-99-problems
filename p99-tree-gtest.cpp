#include <string>
#include "p99-tree.cpp"
#include "lib/gtest-1.7.0/include/gtest/gtest.h"

template<typename T>
void expectEqualTrees(Tree<T>* expected, Tree<T>* actual) {
    std::cout << "expected tree: " << expected->toString() << "\n";
    std::cout << "actual tree:   " << actual->toString() << "\n";
    std::flush(std::cout);
    EXPECT_EQ(*expected, actual);
}

template<typename T>
void expectEqualTrees(List<Tree<T> *> expected, List<Tree<T> *> actual) {
    EXPECT_EQ(expected.size(), actual.size());
    for (auto i = expected.begin(), j = actual.begin(); i != expected.end(); i++, j++) {
        expectEqualTrees(*i, *j);
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

    expectEqualTrees(tree1, tree2);

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
    auto rootNode = node('x');
    auto actual = rootNode->addAllPossibleLeafs('x');

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
    auto actual = constructBalancedTrees(4, 'x');
    auto actual2 = constructBalancedTrees2(4, 'x');

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

TEST(P57, CanAddElementsToTree) {
    auto tree = emptyNode<int>();
    auto actual = tree->addValue(1);
    auto expected = node(1);

    expectEqualTrees(expected, actual);

    deleteAll((List<Tree<int>*>) {tree, actual, expected});
    expectZeroTreeCounter();


    tree = node(2, node(1), node(3));
    actual = tree->addValue(0);
    expected = node(2, node(1, node(0), emptyNode<int>()), node(3));

    expectEqualTrees(expected, actual);

    deleteAll((List<Tree<int>*>) {tree, actual, expected});
    expectZeroTreeCounter();
}

TEST(P57_2, CanConstructTreeFromList) {
    auto tree = fromList((List<int>){5, 3, 18, 1, 4, 12, 21});
    EXPECT_TRUE(tree->isSymmetric());
    delete(tree);

    tree = fromList((List<int>){3, 2, 5, 7, 4});
    EXPECT_FALSE(tree->isSymmetric());
    delete(tree);

    expectZeroTreeCounter();
}

TEST(P58, GenerateAllSymmerticBalancedBinaryTreesWithGivenNumberOfNodes) {
    List<Tree<char>*> expectedTrees = {
            node('x',
                node('x', node('x'), emptyNode<char>()),
                node('x', emptyNode<char>(), node('x'))
            ),
            node('x',
                node('x', emptyNode<char>(), node('x')),
                node('x', node('x'), emptyNode<char>())
            )
    };
    auto actualTrees = symmetricBalancedTrees(5, 'x');

    expectEqualTrees(expectedTrees, actualTrees);

    deleteAll(expectedTrees);
    deleteAll(actualTrees);
    expectZeroTreeCounter();
}

TEST(P59, HeightOfTree) {
    Tree<char>* tree = node('a', node('b'), node('c'));
    EXPECT_EQ(2, tree->height());
    delete(tree);

    tree = node('a', node('b'), node('c', node('d'), emptyNode<char>()));
    EXPECT_EQ(3, tree->height());
    delete(tree);

    expectZeroTreeCounter();
}

TEST(P59, ConstructHeightBalancedTrees) {
    Tree<char>* expected = node('x',
            node('x', node('x'), emptyNode<char>()),
            node('x', node('x'), emptyNode<char>())
    );
    auto allActual = constructHeightBalancedTrees(3, 'x');
    for (auto tree : allActual) std::cout << tree->toString() << "\n";

    EXPECT_EQ(15, allActual.size());
    expectEqualTrees(expected, allActual.front());

    delete(expected);
    deleteAll(allActual);
    expectZeroTreeCounter();
}

TEST(P60, ConstructHeightBalancedTreesWithGivenNumberOfNodes) {
    EXPECT_EQ(4, heightBalancedTreeMinAmountOfNodes(3));
    EXPECT_EQ(7, heightBalancedTreeMinAmountOfNodes(4));
    EXPECT_EQ(12, heightBalancedTreeMinAmountOfNodes(5));

    EXPECT_EQ(3, maxHeightOfHeightBalancedTree(4));
    EXPECT_EQ(3, maxHeightOfHeightBalancedTree(5));
    EXPECT_EQ(3, maxHeightOfHeightBalancedTree(6));
    EXPECT_EQ(4, maxHeightOfHeightBalancedTree(7));

    auto trees = constructHeightBalancedTreesWithNodes(4, 'x');
    for (Tree<char>* tree : trees) std::cout << tree->toString() << "\n";
    EXPECT_EQ(4, trees.size());
    deleteAll(trees);

    trees = constructHeightBalancedTreesWithNodes(15, 'x');
    EXPECT_EQ(2048, trees.size());
    deleteAll(trees);

    expectZeroTreeCounter();
}

TEST(P61, CountTheLeafsOfABinaryTree) {
    Tree<char>* tree = emptyNode<char>();
    EXPECT_EQ(0, tree->leafCount());
    delete(tree);

    tree = node('x', node('x'), node('x'));
    EXPECT_EQ(3, tree->leafCount());
    delete(tree);
}