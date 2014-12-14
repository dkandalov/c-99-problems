#include <string>
#include <sstream>
#include <list>
#include <iostream>
#include <math.h>
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
        left->counter--;
        right->counter--;
        if (left->counter == 0) delete(left);
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
List<Tree<T>*> constructBalancedTrees(int numberOfNodes, T nodeValue) {
    if (numberOfNodes == 0) return {};
    if (numberOfNodes == 1) return { node(nodeValue) };

    List<Tree<T>*> result;
    auto balancedTrees = constructBalancedTrees(numberOfNodes - 1, nodeValue);
    for (auto tree : balancedTrees) {
        for (auto updatedTree : addAllPossibleLeafs(tree, nodeValue)) {
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
List<Tree<T>*> addAllPossibleLeafs(Tree<T>* tree, T nodeValue) {
    if (tree->isEmpty()) return { node(nodeValue) };

    const Node<T>* aNode = dynamic_cast<const Node<T>*>(tree);
    List<Tree<T>*> result;
    for (Tree<T>* leafTree : addAllPossibleLeafs(aNode->left, nodeValue)) {
        result.push_back(node(aNode->value, leafTree, aNode->right));
    }
    for (Tree<T>* leafTree : addAllPossibleLeafs(aNode->right, nodeValue)) {
        result.push_back(node(aNode->value, aNode->left, leafTree));
    }
    return result;
}

template<typename T>
List<Tree<T>*> constructBalancedTrees2(int numberOfNodes, T nodeValue) {
    if (numberOfNodes == 0) return { emptyNode<T>() };
    if (numberOfNodes == 1) return { node(nodeValue) };

    List<Tree<T>*> result;
    auto generateSubtrees = [&] (int leftSize, int rightSize) {
        for (auto leftTree: constructBalancedTrees2(leftSize, nodeValue)) {
            for (auto rightTree : constructBalancedTrees2(rightSize, nodeValue)) {
                result.push_back(node(nodeValue, leftTree, rightTree));
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
List<Tree<T>*> constructHeightBalancedTrees(int height, T value) {
    List<Tree<T>*> allTrees = {emptyNode<T>()};
    int maxAmountOfNodes = (int) (pow(2, height) - 1);

    for (int i = 0; i < maxAmountOfNodes; i++) {
        List<Tree<T>*> updatedTrees = {};
        for (auto tree : allTrees) {
            for (auto updatedTree : addAllPossibleLeafs(tree, value)) {
                if (!contains(updatedTree, updatedTrees)) {
                    updatedTrees.push_back(updatedTree);
                } else {
                    delete(updatedTree);
                }
            }
        }
        // TODO need to check trees with size < maxAmountOfNodes
        deleteAll(allTrees);
        allTrees = updatedTrees;
    }

    List<Tree<T>*> result = {};
    std::cout << "============" << "\n";
    for (auto tree : allTrees) std::cout << tree->toString() << "\n";
    std::cout << "============" << "\n";
    for (auto tree : allTrees) {
        if (tree->isHeightBalanced() && tree->height() == height) {
            result.push_back(tree);
        } else {
            delete(tree);
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
        std::cout << height << "\n";
        for (auto tree : constructHeightBalancedTrees(height, value)) {
            std::cout << tree->toString() << "\n";
            std::flush(std::cout);
            if (tree->size() == amountOfNodes) {
                result.push_back(tree);
            } else {
                delete(tree);
            }
        }
    }
    return result;
}