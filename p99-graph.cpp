#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <set>
#include <AddressBook/AddressBook.h>

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


using String = std::string;
template<typename T>
using Vector = std::vector<T>;
template<typename T>
using Set = std::set<T>;
template<typename T, typename U>
using Map = std::map<T, U>;
template<typename T, typename U>
using Tuple = std::tuple<T, U>;
template<typename T, typename U, typename V>
using Tuple3 = std::tuple<T, U, V>;
template<typename T>
using p = std::unique_ptr<T>;

template<typename T>
p<T> p_(T* t) {
    return p<T>(t);
}

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
        Vector<Edge*> edges;

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
        for (auto edge : node->edges) {
            result.push_back(edgeTarget(edge, node));
        }
        return result;
    }

    // If the edge E connects N to another node, returns
    // the other node, otherwise returns None.
    virtual Node* edgeTarget(Edge* edge, Node* fromNode) const = 0;

    virtual bool equalTo(GraphBase<T, U>* that) {
        bool nodesAreEqual = valuesOf(this->nodes) == valuesOf(that->nodes);

        bool edgesAreEqual = this->edges.size() == that->edges.size();
        if (!edgesAreEqual) return false;
        for (auto thisEdge : this->edges) {
            auto it = std::find_if(that->edges.begin(), that->edges.end(), [&](Edge* thatEdge){
                return (thatEdge->n1->value == thisEdge->n1->value && thatEdge->n2->value == thisEdge->n2->value) ||
                       (thatEdge->n1->value == thisEdge->n2->value && thatEdge->n2->value == thisEdge->n1->value);
            });
            if (it == that->edges.end()) {
                edgesAreEqual = false;
                break;
            }
        }
        return nodesAreEqual && edgesAreEqual;
    }

    void addNode(T value) {
        if (nodes.count(value) != 0) return;
        nodes[value] = new Node(value);
    }

    Tuple<Vector<T>, Vector<Tuple3<T, T, U>>> toTermForm() const {
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
            for (auto edge : node->edges) {
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

    Vector<Vector<T>> findPaths(T fromValue, T toValue) {
        return doFindPaths(fromValue, toValue, Set<T>());
    }

    Vector<Vector<T>> findCycles(T fromValue) {
        Vector<Vector<T>> allPaths;
        for (auto neighbor : neighborsOf(this->nodes[fromValue])) {
            auto paths = findPaths(neighbor->value, fromValue);
            allPaths.insert(allPaths.end(), paths.begin(), paths.end());
        }
        Vector<Vector<T>> result;
        for (auto path : allPaths) {
            if (path.size() < 3) continue;
            path.insert(path.begin(), fromValue);
            result.push_back(path);
        }
        return result;
    }

    virtual String toString() const = 0;

protected:
    template<typename O>
    static std::string convertToString(const O o) {
        std::stringstream s;
        s << o;
        return s.str();
    }

    String toStringWith(String connectionSymbol) const {
        String result = "";
        int counter = 0;
        for (auto entry : nodes) {
            auto node = entry.second;
            if (node->edges.size() == 0) {
                if (counter++ > 0) result += ", ";
                result += convertToString(node->value);

            } else {
                for (int i = 0; i < node->edges.size(); i++) {
                    auto edge = node->edges[i];
                    if (edge->n1->value != node->value) continue;

                    if (counter++ > 0) result += ", ";
                    result += convertToString(edge->n1->value)
                            + connectionSymbol
                            + convertToString(edge->n2->value);

                    auto label = edge->value;
                    if (label != U()) {
                        result += "/" + convertToString(label);
                    }

                }
            }
        }
        return "[" + result + "]";
    }

    static Vector<Tuple3<char, char, int>> parse(String s, char connectionSymbol) {
        Vector<Tuple3<char, char, int>> tokens;
        for (int i = 0; i < s.size();) {
            char node1Value = s[i];
            char node2Value = s[i];
            i += 1;

            if (i + 1 < s.size() && s[i] == connectionSymbol) {
                node2Value = s[i + 1];
                i += 2;
            }

            int label = 0;
            if (i + 1 < s.size() && s[i] == '/') {
                label = s[i + 1] - '0';
                i += 2;
            }
            tokens.push_back(Tuple3<char, char, int>(node1Value, node2Value, label));

            i += 2;
        }
        return tokens;
    }

private:
    Vector<Vector<T>> doFindPaths(T fromValue, T toValue, Set<T> visitedNodes) {
        if (fromValue == toValue) return {{fromValue}};
        auto fromNode = nodes[fromValue];
        auto toNode = nodes[toValue];

        Vector<Vector<T>> result = {};
        auto neighbors = neighborsOf(fromNode);
        visitedNodes.insert(fromValue);

        for (auto neighbor : neighbors) {
            if (visitedNodes.count(neighbor->value) != 0) continue;
            auto paths = doFindPaths(neighbor->value, toValue, visitedNodes);
            for (auto path : paths) {
                path.insert(path.begin(), fromValue);
                result.push_back(path);
            }
        }
        return result;
    }

    Set<T> valuesOf(Map<T, Node*>& map) {
        Set<T> result;
        for (auto item : map) {
            result.insert(item.first);
        }
        return result;
    }
};


template<typename T, typename U>
class Graph : public GraphBase<T, U>, Counted<Graph<T, U>> {
public:
    using Edge = typename GraphBase<T, U>::Edge;
    using Node = typename GraphBase<T, U>::Node;

    bool equalTo(GraphBase<T, U>* that) override {
        return dynamic_cast<Graph*>(that) != NULL && GraphBase<T, U>::equalTo(that);
    }

    Node* edgeTarget(Edge* edge, Node* fromNode) const override {
        if (edge->n1 == fromNode) return edge->n2;
        else if (edge->n2 == fromNode) return edge->n1;
        else return nullptr;
    }

    void addEdge(T n1, T n2, U value) {
        if (this->nodes.count(n1) == 0) throw std::invalid_argument("No nodes for value: " + Graph::convertToString(n1));
        if (this->nodes.count(n2) == 0) throw std::invalid_argument("No nodes for value: " + Graph::convertToString(n2));

        auto node1 = this->nodes[n1];
        auto node2 = this->nodes[n2];
        Vector<Node*> neighbors = this->neighborsOf(node1);
        if (std::find(neighbors.begin(), neighbors.end(), node2) != neighbors.end()) return;

        auto edge = new Edge(node1, node2, value);
        this->edges.push_back(edge);
        node1->edges.push_back(edge);
        node2->edges.push_back(edge);
    }

    p<Graph> minimalSpanningTree() {
        auto graph = new Graph();
        if (this->nodes.empty()) return p_(graph);

        auto startNode = this->edges.front()->n1;
        auto path = minSpanningPath(startNode, Vector<Tuple3<T, T, U>>(), {startNode->value});

        for (auto connection : path) {
            graph->addNode(std::get<0>(connection));
            graph->addNode(std::get<1>(connection));
            graph->addEdge(std::get<0>(connection), std::get<1>(connection), std::get<2>(connection));
        }
        return p_(graph);
    }

    Vector<p<Graph>> allSpanningTrees() {
        if (this->nodes.empty()) return {};

        Vector<p<Graph>> spanningTrees;
        for (auto entry : this->nodes) {
            auto node = entry.second;
            auto spanningPaths = allSpanningPaths(node, Vector<Tuple<T, T>>(), Set<T>({node->value}));

            for (auto path : spanningPaths) {
                auto graph = new Graph();
                for (auto connection : path) {
                    graph->addNode(std::get<0>(connection));
                    graph->addNode(std::get<1>(connection));
                    graph->addEdge(std::get<0>(connection), std::get<1>(connection), U());
                }
                bool isNewGraph = std::find_if(spanningTrees.begin(), spanningTrees.end(), [&](p<Graph>& it) {
                    return it->equalTo(graph);
                }) == spanningTrees.end();
                if (isNewGraph) {
                    spanningTrees.push_back(p_(graph));
                } else {
                    delete(graph);
                }
            }
        }
        return spanningTrees;
    }

    bool isTree() {
        return allSpanningTrees().size() == 1;
    }

    bool isConnected() {
        return allSpanningTrees().size() > 0;
    }

    bool isIsomorphicTo(p<Graph>& graph) {
        if (this->nodes.size() != graph->nodes.size()) return false;
        if (this->edges.size() != graph->edges.size()) return false;

        Set<T> thisValueSet = keySetOf(this->nodes);
        Vector<T> thisValues(thisValueSet.begin(), thisValueSet.end());
        Set<T> valueSet = keySetOf(graph->nodes);
        Vector<T> values(valueSet.begin(), valueSet.end());
        do {
            Graph renamedGraph = Graph();

            for (auto entry : graph->nodes) {
                auto i = std::find(values.begin(), values.end(), entry.first);
                T nodeValue = thisValues[std::distance(values.begin(), i)];
                renamedGraph.addNode(nodeValue);
            }
            for (auto edge : graph->edges) {
                auto i1 = std::find(values.begin(), values.end(), edge->n1->value);
                T nodeValue1 = thisValues[std::distance(values.begin(), i1)];
                auto i2 = std::find(values.begin(), values.end(), edge->n2->value);
                T nodeValue2 = thisValues[std::distance(values.begin(), i2)];
                renamedGraph.addEdge(nodeValue1, nodeValue2, U());
            }
            if (this->equalTo(&renamedGraph)) {
                return true;
            }
        } while (std::next_permutation(values.begin(), values.end()));

        return false;
    };

    int nodeDegreeOf(T nodeValue) {
        if (this->nodes.count(nodeValue) == 0)
            throw std::invalid_argument("No nodes for value: " + Graph::convertToString(nodeValue));
        return (int) this->nodes[nodeValue]->edges.size();
    }

    Vector<T> nodesByDegree() {
        Vector<Node*> nodes;
        for (auto entry : this->nodes) nodes.push_back(entry.second);

        std::sort(nodes.begin(), nodes.end(), [&](Node* n1, Node* n2) {
            return nodeDegreeOf(n2->value) < nodeDegreeOf(n1->value);
        });

        Vector<T> result;
        for (auto node : nodes) result.push_back(node->value);
        return result;
    }

    Vector<Tuple<T, int>> colorNodes() {
        Vector<Tuple<T, int>> result;
        int color = 1;
        auto nodeValues = nodesByDegree();

        while (nodeValues.size() > 0) {
            Vector<T> coloredNodes;
            for (auto nodeValue : nodeValues) {
                if (!isNeighborOf(coloredNodes, nodeValue)) {
                    coloredNodes.push_back(nodeValue);
                }
            }
            for (auto coloredNode : coloredNodes) {
                result.push_back(std::make_tuple(coloredNode, color));
            }
            color++;
            nodeValues.erase(
                std::remove_if(nodeValues.begin(), nodeValues.end(), [&](T value) {
                    return contains(coloredNodes, value);
                }),
                nodeValues.end()
            );
        }
        return result;
    }

    Vector<T> nodesByDepthFrom(T startValue) {
        Vector<T> result;
        Set<T> visited;
        Vector<T> queue = {startValue};
        while (queue.size() > 0) {
            auto nodeValue = queue.back();
            queue.pop_back();
            if (visited.count(nodeValue) > 0) continue;
            visited.insert(nodeValue);
            result.push_back(nodeValue);

            for (auto neighbor : this->neighborsOf(this->nodes[nodeValue])) {
                queue.push_back(neighbor->value);
            }
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    Vector<p<Graph>> connectedComponents() {
        Vector<p<Graph>> result;
        auto allNodeValues = keySetOf(this->nodes);
        while (allNodeValues.size() > 0) {
            auto nodeValues = nodesByDepthFrom(*allNodeValues.begin());
            
            result.push_back(p_(createSubGraphFrom(nodeValues)));

            for (auto nodeValue : nodeValues) {
                allNodeValues.erase(nodeValue);
            }
        }
        return result;
    }

    static p<Graph> term(const Vector<T>& nodeValues, const Vector<Tuple<T, T>>& edgeTuples) {
        Vector<Tuple3<T, T, U>> edges;
        for (Tuple<T, T> tuple : edgeTuples) {
            edges.push_back(Tuple3<T, T, U>(std::get<0>(tuple), std::get<1>(tuple), U()));
        }
        return termLabel(nodeValues, edges);
    }

    static p<Graph> termLabel(const Vector<T>& nodeValues, const Vector<Tuple3<T, T, U>>& edgeTuples) {
        auto graph = p_(new Graph());
        for (auto value : nodeValues) {
            graph->addNode(value);
        }
        for (auto tuple : edgeTuples) {
            graph->addEdge(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
        }
        return graph;
    }

    static p<Graph> adjacent(const Vector<Tuple<T, Vector<T>>>& adjacencyList) {
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

    static p<Graph> adjacentLabel(const Vector<Tuple<T, Vector<Tuple<T, U>>>>& adjacencyList) {
        auto graph = p_(new Graph());
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

    static p<Graph<char, int>> fromString(const String s) {
        auto graph = p_(new Graph<char, int>());

        auto tokens = Graph::parse(s.substr(1, s.size() - 2), '-');
        for (auto token : tokens) {
            graph->addNode(std::get<0>(token));
            graph->addNode(std::get<1>(token));
            if (std::get<0>(token) != std::get<1>(token)) {
                graph->addEdge(std::get<0>(token), std::get<1>(token), std::get<2>(token));
            }
        }
        return graph;
    }

    String toString() const override {
        return Graph::toStringWith("-");
    }

private:
    Vector<Vector<Tuple<T, T>>> allSpanningPaths(Node* node, Vector<Tuple<T, T>> path, Set<T> visited) {
        if (visited.size() == this->nodes.size()) return {path};

        Vector<Vector<Tuple<T, T>>> result;
        for (auto neighbor : this->neighborsOf(node)) {
            if (visited.count(neighbor->value) > 0) continue;

            auto pathCopy = Vector<Tuple<T, T>>(path);
            auto visitedCopy = Set<T>(visited);
            pathCopy.push_back(std::make_tuple(node->value, neighbor->value));
            visitedCopy.insert(neighbor->value);

            for (auto item : allSpanningPaths(neighbor, pathCopy, visitedCopy)) {
                result.push_back(item);
            }
        }
        return result;
    }

    Vector<Tuple3<T, T, U>> minSpanningPath(Node* node, Vector<Tuple3<T, T, U>> path, Set<T> visited) {
        Edge* minEdge = nullptr;
        for (Edge* edge : this->edges) {
            if ((edge->n1->value == node->value || edge->n2->value == node->value)
                    && (minEdge == nullptr || edge->value < minEdge->value)
                    && (visited.count(edge->n1->value) == 0 || visited.count(edge->n2->value) == 0)) {
                minEdge = edge;
            }
        }
        if (minEdge == nullptr) {
            Node* unvisitedNode = nullptr;
            for (auto entry : this->nodes) {
                if (visited.count(entry.first) == 0) {
                    unvisitedNode = entry.second;
                    break;
                }
            }
            if (unvisitedNode == nullptr) return path;
            return minSpanningPath(unvisitedNode, path, visited);
        }

        visited.insert(minEdge->n1->value);
        visited.insert(minEdge->n2->value);
        path.push_back(minEdge->toTuple());
        if (visited.size() == this->nodes.size()) return path;

        T nextNodeValue;
        if (minEdge->n1->value == node->value) nextNodeValue = minEdge->n2->value;
        else nextNodeValue = minEdge->n1->value;

        return minSpanningPath(this->nodes[nextNodeValue], path, visited);
    }

    Set<T> keySetOf(Map<T, Node*> nodeByValue) {
        Set<T> result;
        for (auto value : nodeByValue) {
            result.insert(value.first);
        }
        return result;
    }

    bool contains(Vector<T> vector, T value) {
        return vector.end() != std::find(vector.begin(), vector.end(), value);
    }

    bool isNeighborOf(Vector<T> nodeValues, T nodeValue) {
        for (auto value : nodeValues) {
            for (auto neighbor : this->neighborsOf(this->nodes[value])) {
                if (neighbor->value == nodeValue) return true;
            }
        }
        return false;
    }
    
    Graph* createSubGraphFrom(Vector<T> nodeValues) {
        auto graph = new Graph();
        for (auto nodeValue : nodeValues) {
            graph->addNode(nodeValue);
        }
        for (auto nodeValue : nodeValues) {
            for (auto edge : this->nodes[nodeValue]->edges) {
                graph->addEdge(edge->n1->value, edge->n2->value, edge->value);
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

    bool equalTo(GraphBase<T, U>* that) override {
        return dynamic_cast<Digraph*>(that) != NULL && GraphBase<T, U>::equalTo(that);
    }

    Node* edgeTarget(Edge* edge, Node* fromNode) const override {
        if (edge->n1 == fromNode) return edge->n2;
        else return nullptr;
    }

    void addArc(T source, T dest, U value) {
        auto edge = new Edge(this->nodes[source], this->nodes[dest], value);
        this->edges.push_back(edge);
        this->nodes[source]->edges.push_back(edge);
    }

    static p<Digraph> term(const Vector<T>& nodeValues, const Vector<Tuple<T, T>>& arcTuples) {
        Vector<Tuple3<T, T, U>> updatedArcTuples;
        for (auto tuple : arcTuples) {
            updatedArcTuples.push_back(std::make_tuple(std::get<0>(tuple), std::get<1>(tuple), U()));
        }
        return termLabel(nodeValues, updatedArcTuples);
    }

    static p<Digraph> termLabel(const Vector<T>& nodeValues, const Vector<Tuple3<T, T, U>>& arcTuples) {
        auto graph = p_(new Digraph());
        for (auto value : nodeValues) {
            graph->addNode(value);
        }
        for (auto tuple : arcTuples) {
            graph->addArc(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
        }
        return graph;
    }

    static p<Digraph> adjacent(const Vector<Tuple<T, Vector<T>>>& adjacencyList) {
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

    static p<Digraph> adjacentLabel(const Vector<Tuple<T, Vector<Tuple<T, U>>>>& adjacencyList) {
        auto graph = p_(new Digraph());
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

    static p<Digraph<char, int>> fromString(const String s) {
        auto graph = p_(new Digraph<char, int>());

        auto tokens = Digraph::parse(s.substr(1, s.size() - 2), '>');
        for (auto token : tokens) {
            graph->addNode(std::get<0>(token));
            graph->addNode(std::get<1>(token));
            if (std::get<0>(token) != std::get<1>(token)) {
                graph->addArc(std::get<0>(token), std::get<1>(token), std::get<2>(token));
            }
        }
        return graph;
    }

    String toString() const override {
        return Digraph::toStringWith(">");
    }
};
