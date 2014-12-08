#include <string>
#include <sstream>
#include <list>
#include <iostream>


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

    bool isSymmetric() {
        return true;
    }

    bool isMirrorOf(const Tree<T>* tree) const {
        return (*this) == tree;
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