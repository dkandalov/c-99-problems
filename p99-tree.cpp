#include <string>

// TODO use rule of three/five?

class Tree {
public:
    virtual std::string toString() const = 0;
};

class Node : Tree {
    const int value;
    const Tree* left;
    const Tree* right;

public:
    Node(int value, const Tree* left, const Tree* right):
        value(value), left(left), right(right)
    { }

    virtual ~Node() {
        delete(left);
        delete(right);
    }

    std::string toString() const {
        return "T(" + std::to_string(value) + " " +
                left->toString() + " " + right->toString() + ")";
    }
};

class EmptyNode : Tree {
    std::string toString() const {
        return ".";
    }
};
Tree* emptyNode = new EmptyNode();

//Tree* node(int value) {
//    return new Node(value, )
//}