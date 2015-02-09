#include <vector>
#include <map>
#include <iostream>

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
template<typename T, typename U, typename V>
using Tuple3 = std::tuple<T, U, V>;


template<typename T, typename U>
class GraphBase {
public:
    class Node;
    class Edge : Counted<Edge> {
    public:
        U value;
        Node* n1;
        Node* n2;

        Edge(Node* n1, Node* n2, U value) : n1(n1), n2(n2), value(value) {}

        std::tuple<T, T, U> toTuple() {
            return std::make_tuple(n1->value, n2->value, value);
        }
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

    virtual bool operator ==(GraphBase<T, U>* that) {
        bool nodesAreEqual = (this->nodes.size() == that->nodes.size() &&
            std::equal(this->nodes.begin(), this->nodes.end(), that->nodes.begin(),
                [](decltype(*this->nodes.begin()) a, decltype(*this->nodes.begin()) b) {
                    return a.first == b.first;
                })
        );

        Vector<std::tuple<T, T, U>> thisEdgeTuples(edges.size());
        std::transform(edges.begin(), edges.end(), thisEdgeTuples.begin(), [](Edge* it){return it->toTuple();});
        Vector<std::tuple<T, T, U>> thatEdgeTuples(that->edges.size());
        std::transform(that->edges.begin(), that->edges.end(), thatEdgeTuples.begin(), [](Edge* it){return it->toTuple();});
        bool edgesAreEqual = (edges.size() == that->edges.size() &&
            std::equal(thisEdgeTuples.begin(), thisEdgeTuples.end(), thatEdgeTuples.begin())
        );

        return nodesAreEqual && edgesAreEqual;
    }

    bool operator !=(GraphBase<T, U>* that) {
        return !((*this) == that);
    }

    Node* addNode(T value) {
        auto node = new Node(value);
        nodes[value] = node;
        return node;
    }

    Tuple<Vector<T>, Vector<Tuple3<T, T, U>>> toTermForm() {
        Vector<T> nodeValues;
        for (auto nodeEntry : this->nodes) {
            nodeValues.push_back(nodeEntry.first);
        }

        Vector<Tuple3<T, T, U>> edgeValues;
        for (auto edge : this->edges) {
            edgeValues.push_back(Tuple3<T,T,U>(edge->n1->value, edge->n2->value, edge->value));
        }

        return Tuple<Vector<T>, Vector<Tuple3<T,T,U>>>(nodeValues, edgeValues);
    }

    Vector<Tuple<T, Vector<Tuple<T,U>>>> toAdjacentForm() {
        Vector<Tuple<T, Vector<Tuple<T,U>>>> result;
        for (auto entry : this->nodes) {
            auto node = entry.second;

            Vector<Tuple<T,U>> adjacent;
            for (auto edge : node->adj) {
                if (edge->n1 == node) {
                    adjacent.push_back(Tuple<T,U>(edge->n2->value, edge->value));
                } else {
                    adjacent.push_back(Tuple<T,U>(edge->n1->value, edge->value));
                }
            }
            result.push_back(Tuple<T, Vector<Tuple<T,U>>>(node->value, adjacent));
        }
        return result;
    }
};


template<typename T, typename U>
class Graph : public GraphBase<T, U>, Counted<Graph<T, U>> {
public:
    using Edge = typename GraphBase<T, U>::Edge;
    using Node = typename GraphBase<T, U>::Node;

    bool operator ==(GraphBase<T, U>* that) override {
        return dynamic_cast<Graph*>(that) != NULL && GraphBase<T, U>::operator==(that);
    }

    Node* edgeTarget(Edge* edge, Node* node) const override {
        if (edge->n1 == node) return edge->n2;
        else if (edge->n2 == node) return edge->n1;
        else return nullptr;
    }

    void addEdge(T n1, T n2, U value) {
        if (this->nodes.count(n1) == 0) throw std::invalid_argument("No nodes for value");
        if (this->nodes.count(n2) == 0) throw std::invalid_argument("No nodes for value");

        auto edge = new Edge(this->nodes[n1], this->nodes[n2], value);
        this->edges.push_back(edge);
        this->nodes[n1]->adj.push_back(edge);
        this->nodes[n2]->adj.push_back(edge);
    }

    static Graph* term(const Vector<T>& nodeValues, const Vector<Tuple<T, T>>& edgeTuples) {
        Vector<Tuple3<T, T, U>> edges;
        for (Tuple<T, T> tuple : edgeTuples) {
            edges.push_back(Tuple3<T, T, U>(std::get<0>(tuple), std::get<1>(tuple), U()));
        }
        return termLabel(nodeValues, edges);
    }

    static Graph* termLabel(const Vector<T>& nodeValues, const Vector<Tuple3<T, T, U>>& edgeTuples) {
        auto graph = new Graph();
        for (auto value : nodeValues) {
            graph->addNode(value);
        }
        for (auto tuple : edgeTuples) {
            graph->addEdge(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
        }
        return graph;
    }

    static Graph* adjacent(const Vector<Tuple<T, Vector<T>>>& adjacencyList) {
        Vector<Tuple<T, Vector<Tuple<T, U>>>> list;
        for (auto tuple : adjacencyList) {
            Vector<Tuple<T, U>> newAdjacent;
            for (auto value : std::get<1>(tuple)) {
                newAdjacent.push_back(std::make_tuple(value, U()));
            }
            list.push_back(std::make_tuple(std::get<0>(tuple), newAdjacent));
        }
        return adjacentLabel(list);
    }

    static Graph* adjacentLabel(const Vector<Tuple<T, Vector<Tuple<T, U>>>>& adjacencyList) {
        auto graph = new Graph();
        for (auto tuple : adjacencyList) {
            graph->addNode(std::get<0>(tuple));
        }
        for (auto tuple : adjacencyList) {
            auto nodeValue = std::get<0>(tuple);
            auto adjacent = std::get<1>(tuple);
            for (auto nodeAndLabel : adjacent) {
                auto adjacentNodeValue = std::get<0>(nodeAndLabel);
                auto label = std::get<1>(nodeAndLabel);
                graph->addEdge(nodeValue, adjacentNodeValue, label);
            }
        }
        return graph;
    }
};


template<typename T, typename U>
class Digraph : public GraphBase<T, U> {
public:
    using Edge = typename GraphBase<T, U>::Edge;
    using Node = typename GraphBase<T, U>::Node;

    bool operator ==(GraphBase<T, U>* that) override {
        return dynamic_cast<Digraph*>(that) != NULL && GraphBase<T, U>::operator==(that);
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

    static Digraph* term(const Vector<T>& nodeValues, const Vector<Tuple<T, T>>& arcTuples) {
        Vector<Tuple3<T, T, U>> updatedArcTuples;
        for (auto tuple : arcTuples) {
            updatedArcTuples.push_back(std::make_tuple(std::get<0>(tuple), std::get<1>(tuple), U()));
        }
        return termLabel(nodeValues, updatedArcTuples);
    }

    static Digraph* termLabel(const Vector<T>& nodeValues, const Vector<Tuple3<T, T, U>>& arcTuples) {
        auto graph = new Digraph();
        for (auto value : nodeValues) {
            graph->addNode(value);
        }
        for (auto tuple : arcTuples) {
            graph->addArc(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
        }
        return graph;
    }

    static Digraph* adjacent(const Vector<Tuple<T, Vector<T>>>& adjacencyList) {
        Vector<Tuple<T, Vector<Tuple<T, U>>>> list;
        for (auto tuple : adjacencyList) {
            Vector<Tuple<T, U>> newAdjacent;
            for (auto value : std::get<1>(tuple)) {
                newAdjacent.push_back(std::make_tuple(value, U()));
            }
            list.push_back(std::make_tuple(std::get<0>(tuple), newAdjacent));
        }
        return adjacentLabel(list);
    }

    static Digraph* adjacentLabel(const Vector<Tuple<T, Vector<Tuple<T, U>>>>& adjacencyList) {
        auto graph = new Digraph();
        for (auto tuple : adjacencyList) {
            graph->addNode(std::get<0>(tuple));
        }
        for (auto tuple : adjacencyList) {
            auto nodeValue = std::get<0>(tuple);
            auto adjacentNodeValues = std::get<1>(tuple);
            for (auto nodeAndLabel : adjacentNodeValues) {
                auto adjacentNodeValue = std::get<0>(nodeAndLabel);
                auto label = std::get<1>(nodeAndLabel);
                graph->addArc(nodeValue, adjacentNodeValue, label);
            }
        }
        return graph;
    }
};
