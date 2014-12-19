#include <string>
#include <sstream>
#include <list>
#include <iostream>
#include <CoreFoundation/CoreFoundation.h>


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
    virtual List<Tree<T>*> addAllPossibleLeafs(T value) = 0;
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

    std::string toString() const {
        return "T(" +
            toString(value) + " " +
            left->toString() + " " + right->toString() +
        ")";
    }

private:
    std::string toString(const T t) const {
        std::stringstream s;
        s << t;
        return s.str();
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

    std::string toString() const {
        return ".";
    }
};


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