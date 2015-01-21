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
        Vector<Edge> adj;

        Vector<Node> neighbors() {
            return nullptr;
        }
    };

    Map<T, Node> nodes;
    Vector<Edge> edges;

    bool equals(GraphBase<T, U>* graph) {
        return false;
    }

    Node addNode(T value) {
        auto node = new Node(value);
//        nodes = Map(value -> n) ++ nodes
        return node;
    }
};
