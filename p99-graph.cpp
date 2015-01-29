#include <vector>
#include <map>

// from http://stackoverflow.com/questions/1926605/how-to-count-the-number-of-objects-created-in-c
template<typename T>
struct Counted {
    Counted() {
        objectsCreated++;
        objectsAlive++;
    }

    virtual ~Counted() {
        --objectsAlive;
    }

    static int objectsCreated;
    static int objectsAlive;
};
template<typename T> int Counted<T>::objectsCreated(0);
template<typename T> int Counted<T>::objectsAlive(0);


template<typename T>
using Vector = std::vector<T>;
template<typename T, typename U>
using Map = std::map<T, U>;
template<typename T, typename U>
using Tuple = std::tuple<T, U>;


template<typename T, typename U>
class GraphBase {
public:
    class Node;
    class Edge : Counted<Edge> {
    public:
        U value;
        Node* n1;
        Node* n2;

        Edge(Node* n2, Node* n1, U value) : n2(n2), n1(n1), value(value) {}

        // def toTuple = (n1.value, n2.value, value)
    };
    class Node : Counted<Node> {
    public:
        T value;
        Vector<Edge*> adj;

        Node(T value) : value(value) {}
    };

    Map<T, Node*> nodes;
    Vector<Edge*> edges;


    virtual ~GraphBase() {
        for (auto nodeEntry : nodes) {
            delete(nodeEntry.second);
        }
        for (auto edge : edges) {
            delete(edge);
        }
    }

    Vector<Node*> neighborsOf(Node* node) const {
        Vector<Node*> result;
        for (auto edge : node->adj) {
            result.push_back(edgeTarget(edge, node));
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
        nodes[value] = node;
        return node;
    }
};


template<typename T, typename U>
class Graph : public GraphBase<T, U>, Counted<Graph<T, U>> {
public:
    using Edge = typename GraphBase<T, U>::Edge;
    using Node = typename GraphBase<T, U>::Node;

    bool equals(GraphBase<T, U>* graph) override {
        return false;
    }

    Node* edgeTarget(Edge* edge, Node* node) const override {
        if (edge->n1 == node) return edge->n2;
        else if (edge->n2 == node) return edge->n1;
        else return nullptr;
    }

    void addEdge(T n1, T n2, U value) {
        auto edge = new Edge(this->nodes[n1], this->nodes[n2], value);
        this->edges.push_back(edge);
        this->nodes[n1]->adj.push_back(edge);
        this->nodes[n2]->adj.push_back(edge);
    }

    static Graph* term(const Vector<T>& nodeValues, const Vector<Tuple<T, U>>& edgeValues) {
        auto graph = new Graph();
        for (auto value : nodeValues) {
            graph->addNode(value);
        }
        for (auto tuple : edgeValues) {
            int defaultEdgeLabel = '\0';
            graph->addEdge(std::get<0>(tuple), std::get<1>(tuple), defaultEdgeLabel);
        }
        return graph;
    }

    static Graph* adjacent(const Vector<Tuple<T, Vector<T>>>& adjacencyList) {
        auto graph = new Graph();
        return graph;
    }
};


template<typename T, typename U>
class Digraph : public GraphBase<T, U> {
public:
    using Edge = typename GraphBase<T, U>::Edge;
    using Node = typename GraphBase<T, U>::Node;

    bool equals(GraphBase<T, U>* graph) override {
        return false;
    }

    Node* edgeTarget(Edge* edge, Node* node) const override {
        if (edge->n1 == node) return edge->n2;
        else return nullptr;
    }

    void addArc(T source, T dest, U value) {
        auto edge = new Edge(this->nodes[source], this->nodes[dest], value);
        this->edges.push_back(edge);
        this->nodes[source]->adj.push_back(edge);
    }
};
