#include <string>
#include <sstream>
#include <list>
#include <iostream>
#include <CoreFoundation/CoreFoundation.h>


template<typename T>
using List = std::list<T>;


// TODO use rule of three/five?

int& treeCounter() {
    static int counter = 0;
    return counter;
}

template<typename T>
class Tree {
public:
    virtual ~Tree() { }
    virtual std::string toString() const = 0;
    virtual bool operator==(const Tree<T> *tree) const = 0;
    virtual int size() const = 0;
    bool isEmpty() const {
        return size() == 0;
    }
    virtual bool isBalanced() = 0;
};

template<typename T>
class Node : public Tree<T> {
public:
    const T value;
    const Tree<T>* left;
    const Tree<T>* right;

    Node(T value, const Tree<T>* left, const Tree<T>* right):
        value(value), left(left), right(right)
    {
        treeCounter()++;
    }

    virtual ~Node() {
        treeCounter()--;
        delete(left);
        delete(right);
    }

    bool operator==(const Tree<T>* tree) const {
        const Node<T>* node = dynamic_cast<const Node<T>*>(tree);
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
    virtual ~EmptyNode() {}

    bool operator==(const Tree<T>* tree) const {
        return dynamic_cast<const EmptyNode<T>*>(tree) != NULL;
    }

    int size() const {
        return 0;
    }

    bool isBalanced() {
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
Tree<T>* node(T value, const Tree<T>* left, const Tree<T>* right) {
    return new Node<T>(value, left, right);
}


template<typename T>
List<Tree<T>*> constructBalancedTrees(int numberOfNodes, T nodeValue) {
    if (numberOfNodes == 0) return {};
    if (numberOfNodes == 1) return { node(nodeValue) };

    List<Tree<T>*> result;
    auto balancedTrees = constructBalancedTrees(numberOfNodes - 1, nodeValue);
    for (auto tree : balancedTrees) {
        for (auto updatedTree : addAllPossibleLeafs(tree, nodeValue)) {
            if (updatedTree->isBalanced()) {
                result.push_back(updatedTree);
            }
        }
    }
    result.unique([](const Tree<T>* tree1, const Tree<T>* tree2) -> bool {
        return (*tree1) == tree2;
    });
    return result;
}

template<typename T>
List<Tree<T>*> addAllPossibleLeafs(const Tree<T>* tree, T nodeValue) {
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
