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
void expectEqualTrees(MTree<T>* expected, MTree<T>* actual) {
    std::cout << "expected tree: " << expected->toString() << "\n";
    std::cout << "actual tree:   " << actual->toString() << "\n";
    std::flush(std::cout);
    EXPECT_EQ(*expected, actual);
}

template<typename T>
void expectEqualTrees(List<Tree<T> *> expected, List<Tree<T> *> actual) {
    EXPECT_EQ(expected.size(), actual.size());
    for (auto i = expected.begin(), j = actual.begin();
         i != expected.end() && j != actual.end(); i++, j++) {
        expectEqualTrees(*i, *j);
    }
}

void expectAllTreeNodesToBeDeleted() {
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
    expectAllTreeNodesToBeDeleted();
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
    expectAllTreeNodesToBeDeleted();
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
    expectAllTreeNodesToBeDeleted();
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
    expectAllTreeNodesToBeDeleted();
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

    expectAllTreeNodesToBeDeleted();
}

TEST(P57, CanAddElementsToTree) {
    auto tree = emptyNode<int>();
    auto actual = tree->addValue(1);
    auto expected = node(1);

    expectEqualTrees(expected, actual);

    deleteAll((List<Tree<int>*>) {tree, actual, expected});
    expectAllTreeNodesToBeDeleted();


    tree = node(2, node(1), node(3));
    actual = tree->addValue(0);
    expected = node(2, node(1, node(0), emptyNode<int>()), node(3));

    expectEqualTrees(expected, actual);

    deleteAll((List<Tree<int>*>) {tree, actual, expected});
    expectAllTreeNodesToBeDeleted();
}

TEST(P57_2, CanConstructTreeFromList) {
    auto tree = fromList((List<int>){5, 3, 18, 1, 4, 12, 21});
    EXPECT_TRUE(tree->isSymmetric());
    delete(tree);

    tree = fromList((List<int>){3, 2, 5, 7, 4});
    EXPECT_FALSE(tree->isSymmetric());
    delete(tree);

    expectAllTreeNodesToBeDeleted();
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
    expectAllTreeNodesToBeDeleted();
}

TEST(P59, HeightOfTree) {
    Tree<char>* tree = node('a', node('b'), node('c'));
    EXPECT_EQ(2, tree->height());
    delete(tree);

    tree = node('a', node('b'), node('c', node('d'), emptyNode<char>()));
    EXPECT_EQ(3, tree->height());
    delete(tree);

    expectAllTreeNodesToBeDeleted();
}

TEST(P59, ConstructHeightBalancedTrees) {
    Tree<char>* expected = node('x',
            node('x', node('x'), node('x')),
            node('x', node('x'), node('x'))
    );
    auto allActual = constructHeightBalancedTrees(3, 'x');

    for (auto tree : allActual) std::cout << tree->toString() << "\n";
    EXPECT_EQ(15, allActual.size());
    expectEqualTrees(expected, allActual.front());

    delete(expected);
    deleteAll(allActual);
    expectAllTreeNodesToBeDeleted();
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
    EXPECT_EQ(1553, trees.size());
    deleteAll(trees);

    expectAllTreeNodesToBeDeleted();
}

TEST(P61, CountTheLeafsOfABinaryTree) {
    Tree<char>* tree = emptyNode<char>();
    EXPECT_EQ(0, tree->leafCount());
    delete(tree);

    tree = node('x', node('x'), node('x'));
    EXPECT_EQ(3, tree->leafCount());
    delete(tree);

    expectAllTreeNodesToBeDeleted();
}

TEST(P61A, CollectLeavesOfABinaryTreeIntoAList) {
    Tree<char>* tree = emptyNode<char>();
    List<char> expected = {};
    EXPECT_EQ(expected, tree->leafList());
    delete(tree);

    tree = node('a', node('b'), node('c', node('d'), emptyNode<char>()));
    expected = {'b', 'd'};
    EXPECT_EQ(expected, tree->leafList());
    delete(tree);

    expectAllTreeNodesToBeDeleted();
}

TEST(P62, CollectInternalNodesOfABinaryTreeIntoAList) {
    Tree<char>* tree = emptyNode<char>();
    List<char> expected = {};
    EXPECT_EQ(expected, tree->internalList());
    delete(tree);

    tree = node('a', node('b'), node('c', node('d'), emptyNode<char>()));
    expected = {'a', 'c'};
    EXPECT_EQ(expected, tree->internalList());
    delete(tree);

    expectAllTreeNodesToBeDeleted();
}

TEST(P63, ConstructCompleteBinaryTree) {
    Tree<char>* expected = node('x',
            node('x', node('x'), node('x')),
            node('x', node('x'), emptyNode<char>())
    );
    auto actual = completeBinaryTree(6, 'x');
    expectEqualTrees(expected, actual);

    delete(expected);
    delete(actual);
    expectAllTreeNodesToBeDeleted();
}

TEST(P64, LayoutABinaryTree1) {
    Tree<char>* tree = node('a',
            node('b', emptyNode<char>(), node('c')),
            node('d')
    );
    Tree<char>* layoutTree = tree->layout();
    EXPECT_EQ("T[3,1](a T[1,2](b . T[2,3](c . .)) T[4,2](d . .))", layoutTree->toString());

    delete(tree);
    delete(layoutTree);
    expectAllTreeNodesToBeDeleted();
}

TEST(P64, LayoutABinaryTree1_ComplexTest) {
    Tree<char>* tree = fromList<char>({'n','k','m','c','a','h','g','e','u','p','s','q'});
    Tree<char>* layoutTree = tree->layout();

    EXPECT_EQ(
            "T[8,1](n "
                    "T[6,2](k "
                        "T[2,3](c T[1,4](a . .) T[5,4](h T[4,5](g T[3,6](e . .) .) .)) "
                        "T[7,3](m . .)) "
                    "T[12,2](u "
                        "T[9,3](p . T[11,4](s T[10,5](q . .) .)) .))",
            layoutTree->toString()
    );

    delete(tree);
    delete(layoutTree);
    expectAllTreeNodesToBeDeleted();
}

TEST(P65, LayoutABinaryTree2) {
    Tree<char>* tree = node('a',
            node('b', emptyNode<char>(), node('c')),
            node('d')
    );
    Tree<char>* layoutTree = tree->layout2();
    EXPECT_EQ("T[3,1](a T[1,2](b . T[2,3](c . .)) T[5,2](d . .))", layoutTree->toString());

    delete(tree);
    delete(layoutTree);
    expectAllTreeNodesToBeDeleted();
}

TEST(P65, LayoutABinaryTree2_ComplexTest) {
    Tree<char>* tree = fromList<char>({'n','k','m','c','a','e','d','g','u','p','q'});
    Tree<char>* layoutTree = tree->layout2();

    EXPECT_EQ(
            "T[15,1](n "
                "T[7,2](k "
                    "T[3,3](c "
                        "T[1,4](a . .) "
                        "T[5,4](e T[4,5](d . .) T[6,5](g . .))"
                    ") "
                    "T[11,3](m . .)"
                ") "
                "T[23,2](u "
                    "T[19,3](p . T[21,4](q . .)) "
                    "."
                ")"
            ")",
            layoutTree->toString()
    );

    delete(tree);
    delete(layoutTree);
    expectAllTreeNodesToBeDeleted();
}

TEST(P66, LayoutABinaryTree3_SimpleCase) {
    Tree<char>* tree = node('a',
            node('b', emptyNode<char>(), node('c')),
            node('d')
    );
    Tree<char>* layoutTree = tree->layout3();
    EXPECT_EQ("T[2,1](a T[1,2](b . T[2,3](c . .)) T[3,2](d . .))", layoutTree->toString());

    delete(tree);
    delete(layoutTree);
    expectAllTreeNodesToBeDeleted();
}

TEST(P66, LayoutABinaryTree3_CaseWithCollision) {
    Tree<char>* tree = node('a',
            node('b', emptyNode<char>(), node('c')),
            node('d', node('e'), emptyNode<char>())
    );
    Tree<char>* layoutTree = tree->layout3();
    EXPECT_EQ("T[3,1](a "
                "T[1,2](b . T[2,3](c . .)) "
                "T[5,2](d T[4,3](e . .) .))",
            layoutTree->toString()
    );

    delete(tree);
    delete(layoutTree);
    expectAllTreeNodesToBeDeleted();
}

TEST(P66, LayoutABinaryTree3_ComplexCase) {
    Tree<char>* tree = node('n',
            node('k',
                    node('c',
                            node('a'),
                            node('e', node('d'), node('g'))),
                    node('m')),
            node('u',
                    node('p',
                            emptyNode<char>(),
                            node('q')),
                    emptyNode<char>())
    );
    Tree<char>* layoutTree = tree->layout3();
    EXPECT_EQ(""
            "T[5,1](n "
              "T[3,2](k "
                "T[2,3](c "
                  "T[1,4](a . .) "
                  "T[3,4](e T[2,5](d . .) T[4,5](g . .))) T[4,3](m . .)) "
              "T[7,2](u "
                "T[6,3](p . T[7,4](q . .)) .))",
            layoutTree->toString()
    );

    delete(tree);
    delete(layoutTree);
    expectAllTreeNodesToBeDeleted();
}

TEST(P67, StringRepresentationOfBinaryTrees) {
    Tree<char>* expected = node('a',
            node('b',
                    node('d'),
                    node('e')),
            node('c',
                    emptyNode<char>(),
                    node('f', node('g'), emptyNode<char>()))
    );

    EXPECT_EQ("a(b(d,e),c(,f(g,)))", expected->asString());
    Tree<char>* actual = Tree<char>::fromString("a(b(d,e),c(,f(g,)))");
    expectEqualTrees(expected, actual);

    delete(expected);
    delete(actual);
    expectAllTreeNodesToBeDeleted();
}

TEST(P68, PreorderAndInorderSequencesOfABinaryTrees_PartA) {
    auto tree = Tree<char>::fromString("a(b(d,e),c(,f(g,)))");

    List<char> expected = {'a', 'b', 'd', 'e', 'c', 'f', 'g'};
    EXPECT_EQ(expected, tree->preorder());

    expected = {'d', 'b', 'e', 'a', 'c', 'g', 'f'};
    EXPECT_EQ(expected, tree->inorder());

    delete(tree);
    expectAllTreeNodesToBeDeleted();
}

TEST(P68, PreorderAndInorderSequencesOfABinaryTrees_PartB) {
    auto expected = node('a', node('b'), node('c'));
    auto actual = Tree<char>::sequenceToTree(
            {'a', 'b', 'c'},
            {'b', 'a', 'c'}
    );

    expectEqualTrees(expected, actual);

    delete(expected);
    delete(actual);
    expectAllTreeNodesToBeDeleted();
}

TEST(P68, PreorderAndInorderSequencesOfABinaryTrees_PartB_ComplexCase) {
    auto expected =
            node('a',
                    node('b',
                            node('d'),
                            node('e')),
                    node('c',
                            emptyNode<char>(),
                            node('f',
                                    node('g'),
                                    emptyNode<char>()
                            )
                    )
            );
    auto actual = Tree<char>::sequenceToTree(
            {'a', 'b', 'd', 'e', 'c', 'f', 'g'},
            {'d', 'b', 'e', 'a', 'c', 'g', 'f'}
    );

    expectEqualTrees(expected, actual);

    delete(expected);
    delete(actual);
    expectAllTreeNodesToBeDeleted();
}

TEST(P69, DotStringRepresentationOfBinaryTrees) {
    auto expected =
            node('a',
                    node('b',
                            node('d'),
                            node('e')),
                    node('c',
                            emptyNode<char>(),
                            node('f',
                                    node('g'),
                                    emptyNode<char>()
                            )
                    )
            );
    
    EXPECT_EQ("abd..e..c.fg...", expected->toDotString());

    Tree<char>* actual = Tree<char>::fromDotString("abd..e..c.fg...");
    expectEqualTrees(expected, actual);

    delete(actual);
    delete(expected);
    expectAllTreeNodesToBeDeleted();
}

TEST(P70A, CanCreateMultiwayTrees) {
    auto tree = new MTree<char>('a');
    EXPECT_EQ('a', tree->value);
    EXPECT_EQ(0, tree->children.size());
    delete(tree);

    tree = new MTree<char>('a', {
            new MTree<char>('b'), new MTree<char>('c'), new MTree<char>('d')
    });
    EXPECT_EQ('a', tree->value);
    EXPECT_EQ(3, tree->children.size());
    delete(tree);

    expectAllTreeNodesToBeDeleted();
}

TEST(P70C, CountAmountOfNodesInMultiwayTree) {
    auto tree = new MTree<char>('a');
    EXPECT_EQ(1, tree->nodeCount());
    delete(tree);

    tree = new MTree<char>('a', {
            new MTree<char>('b'), new MTree<char>('c'), new MTree<char>('d')
    });
    EXPECT_EQ(4, tree->nodeCount());
    delete(tree);

    expectAllTreeNodesToBeDeleted();
}

TEST(P70, TreeConstructionFromNodeString) {
    auto actual = MTree<char>::stringToMTree("a^");
    auto expected = new MTree<char>('a');
    expectEqualTrees(expected, actual);
    delete(actual);
    delete(expected);

    actual = MTree<char>::stringToMTree("ab^c^d^^");
    expected = new MTree<char>('a', {
        new MTree<char>('b'),
        new MTree<char>('c'),
        new MTree<char>('d')
    });
    expectEqualTrees(expected, actual);
    delete(actual);
    delete(expected);

    actual = MTree<char>::stringToMTree("afg^^c^bd^e^^^");
    expected = new MTree<char>('a', {
        new MTree<char>('f', { new MTree<char>('g') }),
        new MTree<char>('c'),
        new MTree<char>('b', { new MTree<char>('d'), new MTree<char>('e') })
    });
    expectEqualTrees(expected, actual);
    delete(actual);
    delete(expected);

    expectAllTreeNodesToBeDeleted();
}

TEST(P71, InternalPathLengthOfATree) {
    auto tree = MTree<char>::stringToMTree("a^");
    EXPECT_EQ(0, tree->internalPathLength());
    delete(tree);

    tree = new MTree<char>('a', {
            new MTree<char>('f', { new MTree<char>('g') }),
            new MTree<char>('c'),
            new MTree<char>('b', { new MTree<char>('d'), new MTree<char>('e') })
    });
    EXPECT_EQ(9, tree->internalPathLength());
    delete(tree);

    expectAllTreeNodesToBeDeleted();
}

TEST(P72, ConstructPostOrderSequenceOfTreeNodes) {
    auto tree = new MTree<char>('a', {
            new MTree<char>('f', { new MTree<char>('g') }),
            new MTree<char>('c'),
            new MTree<char>('b', { new MTree<char>('d'), new MTree<char>('e') })
    });
    List<char> expected = {'g', 'f', 'c', 'd', 'e', 'b', 'a'};

    EXPECT_EQ(expected, tree->postorder());

    delete(tree);
    expectAllTreeNodesToBeDeleted();
}

TEST(P73, LispLikeTreeRepresentation) {
    auto tree = new MTree<char>('a', {
            new MTree<char>('f', { new MTree<char>('g') }),
            new MTree<char>('c'),
            new MTree<char>('b', { new MTree<char>('d'), new MTree<char>('e') })
    });

    EXPECT_EQ("(a (f g) c (b d e))", tree->lispyTree());

    delete(tree);
    expectAllTreeNodesToBeDeleted();
}