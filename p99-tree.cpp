#include <string>
#include <sstream>
#include <list>
#include <iostream>
#include <cmath>
#include <CoreGraphics/CoreGraphics.h>


template<typename T>
using List = std::list<T>;


// TODO use rule of three/five?

int& treeNodesCounter() {
    static int amountOfNodes = 0;
    return amountOfNodes;
}

class UsageCounter {
public:
    int counter;
    UsageCounter() : counter(0) {}
};

template<typename T>
class Tree : public UsageCounter {
public:
    virtual ~Tree() { }
    virtual std::string toString() const = 0;
    virtual bool operator==(const Tree<T> *tree) const = 0;
    virtual int size() const = 0;
    bool isEmpty() const {
        return size() == 0;
    }
    virtual bool isBalanced() = 0;
    virtual bool isSymmetric() = 0;
    virtual bool isMirrorOf(const Tree<T>* tree) const = 0;
    virtual Tree<T>* addValue(T value) = 0;
    virtual int height() = 0;
    virtual bool isHeightBalanced() = 0;
    virtual int leafCount() = 0;
    virtual List<T> leafList() = 0;
    virtual List<T> internalList() = 0;
    virtual List<Tree<T>*> addAllPossibleLeafs(T value) = 0;
    virtual Tree<T>* layout() = 0;
    virtual Tree<T>* layoutWithShift(int xShift, int yShift) = 0;
    virtual Tree<T>* layout2() = 0;
    virtual Tree<T>* layout2WithShift(int x, int y, int leftXShift, int level) = 0;
    virtual Tree<T>* layout3() = 0;
    virtual Tree<T>* layout3WithShift(int x, int y, bool parentX) = 0;
    virtual int width() = 0;
};

template<typename T>
void deleteAll(List<Tree<T>*> listOfTrees) {
    for (auto tree : listOfTrees) delete(tree);
}


template<typename T>
class Node : public Tree<T> {
public:
    const T value;
    Tree<T>* left;
    Tree<T>* right;

    Node(T value, Tree<T>* left, Tree<T>* right):
        value(value), left(left), right(right)
    {
        treeNodesCounter()++;
        left->counter++;
        right->counter++;
    }

    virtual ~Node() {
        treeNodesCounter()--;

        // it's important to attempt deletion of "left"
        // before "right" in case it's the same object
        left->counter--;
        if (left->counter == 0) delete(left);
        right->counter--;
        if (right->counter == 0) delete(right);
    }

    bool operator==(const Tree<T>* tree) const {
        auto node = dynamic_cast<const Node<T>*>(tree);
        return node != NULL &&
                value == node->value &&
                (*left) == node->left &&
                (*right) == node->right;
    }

    int size() const {
        return 1 + left->size() + right->size();
    }

    bool isBalanced() {
        return abs(left->size() - right->size()) <= 1;
    }

    bool isSymmetric() {
        return left->isMirrorOf(right);
    }

    bool isMirrorOf(const Tree<T>* tree) const {
        auto that = dynamic_cast<const Node<T>*>(tree);
        return that != NULL &&
                this->left->isMirrorOf(that->right) &&
                this->right->isMirrorOf(that->left);
    }

    Tree<T>* addValue(T value) {
        if (value < this->value) {
            return node(this->value, left->addValue(value), right);
        } else {
            return node(this->value, left, right->addValue(value));
        }
    }

    int height() {
        return 1 + std::max(left->height(), right->height());
    }

    bool isHeightBalanced() {
        bool thisIsBalanced = abs(left->height() - right->height()) <= 1;
        return thisIsBalanced && left->isHeightBalanced() && right->isHeightBalanced();
    }

    List<Tree<T>*> addAllPossibleLeafs(T value) {
        List<Tree<T>*> result;
        for (Tree<T>* leafTree : left->addAllPossibleLeafs(value)) {
            result.push_back(node(value, leafTree, right));
        }
        for (Tree<T>* leafTree : right->addAllPossibleLeafs(value)) {
            result.push_back(node(value, left, leafTree));
        }
        return result;
    }

    int leafCount() {
        return 1 + left->leafCount() + right->leafCount();
    }

    List<T> leafList() {
        if (left->size() == 0 && right->size() == 0) {
            return {value};
        } else {
            List<T> result = left->leafList();
            result.splice(result.end(), right->leafList());
            return result;
        }
    }

    List<T> internalList() {
        if (left->size() == 0 && right->size() == 0) {
            return {};
        } else {
            List<T> result = {value};
            result.splice(result.end(), left->internalList());
            result.splice(result.end(), right->internalList());
            return result;
        };
    }

    Tree<T>* layout();
    Tree<T>* layoutWithShift(int xShift, int yShift);

    Tree<T>* layout2();
    Tree<T>* layout2WithShift(int x, int y, int leftXShift, int level);

    Tree<T>* layout3();
    Tree<T>* layout3WithShift(int x, int y, bool leftmostXIsFixed);

    int width() {
        return 1 + left->width() + right->width();
    }

    std::string toString() const {
        return "T(" +
                asString(value) + " " +
                left->toString() + " " + right->toString() +
        ")";
    }

protected:
    std::string asString(const T t) const {
        std::stringstream s;
        s << t;
        return s.str();
    }
};


template<typename T>
class PositionedNode: public Node<T> {

public:
    const int x;
    const int y;

    PositionedNode(T value, Tree<T>* left, Tree<T>* right, int x, int y) :
            Node<T>(value, left, right), x(x), y(y) {}

    std::string toString() const {
        return "T[" + std::to_string(x) + "," + std::to_string(y) + "]" +
                "(" + this->asString(this->value) + " " +
                this->left->toString() + " " + this->right->toString() +
                ")";
    }
};


template<typename T>
class EmptyNode : public Tree<T> {
public:
    EmptyNode() {
        treeNodesCounter()++;
    }

    virtual ~EmptyNode() {
        treeNodesCounter()--;
    }

    bool operator==(const Tree<T>* tree) const {
        return dynamic_cast<const EmptyNode<T>*>(tree) != NULL;
    }

    int size() const {
        return 0;
    }

    bool isBalanced() {
        return true;
    }

    bool isSymmetric() {
        return true;
    }

    bool isMirrorOf(const Tree<T>* tree) const {
        return (*this) == tree;
    }

    Tree<T>* addValue(T value) {
        return new Node<T>(value, new EmptyNode<T>(), new EmptyNode<T>());
    }

    int height() {
        return 0;
    }

    bool isHeightBalanced() {
        return true;
    }

    List<Tree<T>*> addAllPossibleLeafs(T value) {
        return { new Node<T>(value, new EmptyNode<T>(), new EmptyNode<T>()) };
    }

    int leafCount() {
        return 0;
    }

    List<T> leafList() {
        return {};
    }

    List<T> internalList() {
        return {};
    }

    Tree<T>* layout() {
        return this;
    }

    Tree<T>* layoutWithShift(int xShift, int yShift) {
        return this;
    }

    Tree<T>* layout2() {
        return this;
    }

    Tree<T>* layout2WithShift(int xShift, int yShift, int leftXShift, int level) {
        return this;
    }

    Tree<T>* layout3() {
        return this;
    }

    Tree<T>* layout3WithShift(int x, int y, bool leftmostXIsFixed) {
        return this;
    }

    int width() {
        return 0;
    }

    std::string toString() const {
        return ".";
    }
};


template<typename T>
Tree<T>* Node<T>::layout() {
    return layoutWithShift(1, 1);
}
template<typename T>
Tree<T>* Node<T>::layoutWithShift(int xShift, int yShift) {
    auto leftLayout = left->layoutWithShift(xShift, yShift + 1);
    auto rightLayout = right->layoutWithShift(xShift + leftLayout->width() + 1, yShift + 1);
    return new PositionedNode<T>(
            value,
            leftLayout,
            rightLayout,
            xShift + leftLayout->width(),
            yShift
    );
}

template<typename T>
Tree<T>* Node<T>::layout2() {
    int xShift = 1;
    int yShift = 1;

    int level = this->height() - 2;
    auto tree = this->left;
    while (tree->size() > 0) {
        int levelShift = level < 0 ? 0 : (int) pow(2, level);

        xShift += levelShift;

        auto treeNode = dynamic_cast<Node<T>*>(tree);
        if (treeNode == NULL) break;
        tree = treeNode->left;
        level--;
    }

    return layout2WithShift(xShift, yShift, -1, this->height() - 2);
}
template<typename T>
Tree<T>* Node<T>::layout2WithShift(int x, int y, int leftXShift, int level) {
    int levelShift = level < 0 ? 0 : (int) pow(2, level);

    auto leftLayout = left->layout2WithShift(x - levelShift, y + 1, leftXShift, level - 1);
    auto rightLayout = right->layout2WithShift(x + levelShift, y + 1, leftXShift, level - 1);

    return new PositionedNode<T>(value, leftLayout, rightLayout, x, y);
}

template<typename T>
Tree<T>* Node<T>::layout3() {
    return layout3WithShift(1, 1, false);
}

template<typename T>
Tree<T>* Node<T>::layout3WithShift(int x, int y, bool leftmostXIsFixed) {
    Tree<T>* leftLayout;
    Tree<T>* rightLayout;
    if (leftmostXIsFixed) {
        leftLayout = left->layout3WithShift(x - 1, y + 1, leftmostXIsFixed);
        rightLayout = right->layout3WithShift(x + 1, y + 1, leftmostXIsFixed);
    } else {
        leftLayout = left->layout3WithShift(x, y + 1, leftmostXIsFixed);
        x = leftLayout->isEmpty() ? x : (dynamic_cast<PositionedNode<T>*>(leftLayout))->x + 1;
        leftmostXIsFixed = true;
        rightLayout = right->layout3WithShift(x + 1, y + 1, leftmostXIsFixed);
    }

    // TODO detect node position collision

    return new PositionedNode<T>(value, leftLayout, rightLayout, x, y);
}


template<typename T>
Tree<T>* emptyNode() {
    return new EmptyNode<T>();
}

template<typename T>
Tree<T>* node(T value) {
    return new Node<T>(value, emptyNode<T>(), emptyNode<T>());
}

template<typename T>
Tree<T>* node(T value, Tree<T>* left, Tree<T>* right) {
    return new Node<T>(value, left, right);
}

template<typename T>
bool contains(Tree<T>* tree, List<Tree<T>*> list) {
    return std::find_if(list.begin(), list.end(), [&](const Tree<T>* aTree) -> bool {
        return (*tree) == aTree;
    }) != list.end();
}

template<typename T>
Tree<T>* fromList(List<T> values) {
    Tree<T>*tree = emptyNode<T>();
    for (auto value : values) {
        auto newTree = tree->addValue(value);
        delete(tree);
        tree = newTree;
    }
    return tree;
}


template<typename T>
List<Tree<T>*> constructBalancedTrees(int numberOfNodes, T value) {
    if (numberOfNodes == 0) return {};
    if (numberOfNodes == 1) return { node(value) };

    List<Tree<T>*> result;
    auto balancedTrees = constructBalancedTrees(numberOfNodes - 1, value);
    for (auto tree : balancedTrees) {
        for (auto updatedTree : tree->addAllPossibleLeafs(value)) {
            if (updatedTree->isBalanced() && !contains(updatedTree, result)) {
                result.push_back(updatedTree);
            } else {
                delete(updatedTree);
            }
        }
    }
    deleteAll(balancedTrees);
    return result;
}

template<typename T>
List<Tree<T>*> constructBalancedTrees2(int numberOfNodes, T value) {
    if (numberOfNodes == 0) return { emptyNode<T>() };
    if (numberOfNodes == 1) return { node(value) };

    List<Tree<T>*> result;
    auto generateSubtrees = [&] (int leftSize, int rightSize) {
        for (auto leftTree: constructBalancedTrees2(leftSize, value)) {
            for (auto rightTree : constructBalancedTrees2(rightSize, value)) {
                result.push_back(node(value, leftTree, rightTree));
            }
        }
    };

    if (numberOfNodes % 2 == 1) {
        generateSubtrees(numberOfNodes / 2, numberOfNodes / 2);
    } else {
        generateSubtrees(numberOfNodes / 2, (numberOfNodes / 2) - 1);
        generateSubtrees((numberOfNodes / 2) - 1, numberOfNodes / 2);
    }
    return result;
}

template<typename T>
List<Tree<T>*> symmetricBalancedTrees(int numberOfNodes, T value) {
    List<Tree<T>*> allTrees = {emptyNode<T>()};
    for (int i = 0; i < numberOfNodes; i++) {
        List<Tree<T>*> trees;
        for (auto tree : allTrees) {
            trees.splice(trees.end(), tree->addAllPossibleLeafs(value));
        }
        deleteAll(allTrees);
        allTrees = trees;
    }

    List<Tree<T>*> result;
    for (auto tree : allTrees) {
        if (tree->isSymmetric() && !(contains(tree, result))) {
            result.push_back(tree);
        } else {
            delete(tree);
        }
    }
    return result;
}

template<typename T>
List<Tree<T>*> constructHeightBalancedTrees(int height, T value) {
    if (height == 0) return {emptyNode<T>()};
    if (height == 1) return {node(value)};
    List<Tree<T>*> result = {};

    List<Tree<T> *> trees = constructHeightBalancedTrees(height - 1, value);
    List<Tree<T> *> smallerTrees = constructHeightBalancedTrees(height - 2, value);
    for (auto tree1 : trees) {
        for (auto tree2 : trees) {
            result.push_back(node(value, tree1, tree2));
        }
    }
    for (auto tree1 : trees) {
        for (auto tree2 : smallerTrees) {
            result.push_back(node(value, tree1, tree2));
            result.push_back(node(value, tree2, tree1));
        }
    }
    return result;
}

int heightBalancedTreeMinAmountOfNodes(int height) {
    if (height == 0) return 0;
    if (height == 1) return 1;
    if (height == 2) return 2;
    return heightBalancedTreeMinAmountOfNodes(height - 1) +
           heightBalancedTreeMinAmountOfNodes(height - 2) + 1;
}

int minHeightOfHeightBalancedTree(int amountOfNodes) {
    return amountOfNodes == 0 ? 0 : minHeightOfHeightBalancedTree(amountOfNodes / 2) + 1;
}

int maxHeightOfHeightBalancedTree(int amountOfNodes) {
    int height = 1;
    while (amountOfNodes >= heightBalancedTreeMinAmountOfNodes(height)) {
        height++;
    }
    return height - 1;
}

template<typename T>
List<Tree<T>*> constructHeightBalancedTreesWithNodes(int amountOfNodes, T value) {
    List<Tree<T>*> result;
    int minHeight = minHeightOfHeightBalancedTree(amountOfNodes);
    int maxHeight = maxHeightOfHeightBalancedTree(amountOfNodes);

    for (int height = minHeight; height <= maxHeight; height++) {
        for (auto tree : constructHeightBalancedTrees(height, value)) {
            if (tree->size() == amountOfNodes) {
                result.push_back(tree);
            } else {
                delete(tree);
            }
        }
    }
    return result;
}

template<typename T>
Tree<T>* completeBinaryTree(int index, int amountOfNodes, T value) {
    if (index > amountOfNodes) return emptyNode<T>();
    else {
        return new Node<T>(value,
                completeBinaryTree(index * 2, amountOfNodes, value),
                completeBinaryTree(index * 2 + 1, amountOfNodes, value));
    }
}

template<typename T>
Tree<T>* completeBinaryTree(int amountOfNodes, T value) {
    if (amountOfNodes == 0) return emptyNode<T>();
    return completeBinaryTree(1, amountOfNodes, value);
}
