#include <string>
#include <sstream>
#include <list>


template<typename T>
using List = std::list<T>;


// TODO use rule of three/five?

template<typename T>
class Tree {
public:
    virtual ~Tree() { }
    virtual std::string toString() const = 0;
    virtual bool operator==(const Tree<T> *tree) const = 0;
};

template<typename T>
class Node : public Tree<T> {
    const T value;
    const Tree<T>* left;
    const Tree<T>* right;

public:
    Node(T value, const Tree<T>* left, const Tree<T>* right):
        value(value), left(left), right(right)
    {}

    virtual ~Node() {
        delete(left);
        delete(right);
    }

    bool operator==(const Tree<T>* tree) const {
        const Node* node = dynamic_cast<const Node*>(tree);
        return node != NULL &&
                value == node->value &&
                (*left) == node->left &&
                (*right) == node->right;
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
        return dynamic_cast<const EmptyNode*>(tree) != NULL;
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
List<Tree<T>*> constructBalancedTrees(int numberOfNodes, T value) {
    return { emptyNode<T>() };
}
