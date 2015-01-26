#include <vector>
#include <map>

template<typename T>
using Vector = std::vector<T>;
template<typename T, typename U>
using Map = std::map<T, U>;


template<typename T, typename U>
class GraphBase {
public:
    class Node;
    class Edge {
        U value;
        Node n1;
        Node n2;
        // def toTuple = (n1.value, n2.value, value)
    };
    class Node {
        T value;
        Vector<Edge*> adj;
    };

    Map<T, Node> nodes;
    Vector<Edge> edges;

    Vector<Node*> neighborsOf(Node* node) const {
        Vector<Node*> result;
        for (auto edge : node->adj) {
            result.push_back(edgeTarget(edge, this));
        }
        return result;
    }

    // If the edge E connects N to another node, returns
    // the other node, otherwise returns None.
    virtual Node* edgeTarget(Edge* edge, Node* node) const = 0;

    virtual bool equals(GraphBase<T, U>* graph) {
        return false;
    }

    Node* addNode(T value) {
        auto node = new Node(value);
//        nodes = Map(value -> n) ++ nodes
        return node;
    }
};

template<typename T, typename U>
class Graph : public GraphBase<T, U> {
public:
    bool equals(GraphBase<T, U>* graph) override {
        return false;
    }

    typename GraphBase<T, U>::Node* edgeTarget(
            typename GraphBase<T, U>::Edge* edge,
            typename GraphBase<T, U>::Node* node) const override {

        return nullptr;
    }

    void addEdge(T n1, T n2, U value) {
    }
};

template<typename T, typename U>
class Digraph : public GraphBase<T, U> {

public:
    bool equals(GraphBase<T, U>* graph) override {
        return false;
    }

    typename GraphBase<T, U>::Node* edgeTarget(
            typename GraphBase<T, U>::Edge* edge,
            typename GraphBase<T, U>::Node* node) const override {

        return nullptr;
    }

    void addArc(T source, T dest, U value) {
    }
};


