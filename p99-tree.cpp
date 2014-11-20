#include <string>

// TODO use rule of three/five?

template<typename T>
class Tree {
public:
    virtual ~Tree() {}
    virtual std::string toString() const = 0;
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

    std::string toString() const {
        return "T(" +
            std::to_string(value) + " " +
            left->toString() + " " + right->toString() +
        ")";
    }
};

template<typename T>
class EmptyNode : public Tree<T> {
public:
    virtual ~EmptyNode() {}
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
