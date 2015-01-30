#include <gtest/gtest.h>
#include "p99-graph.cpp"

using CharGraph = Graph<char, char>;
using CharDigraph = Digraph<char, char>;
using CharTuple = Tuple<char, char>;
using CharAdjacency = Tuple<char, Vector<char>>;

void expectAllGraphObjectsToBeDeleted() {
//    std::cout << "graphs created: " << Counted<CharGraph>::objectsCreated << "\n";
//    std::cout << "nodes created: " << Counted<CharGraph::Node>::objectsCreated << "\n";
//    std::cout << "edges created: " << Counted<CharGraph::Edge>::objectsCreated << "\n";
    EXPECT_EQ(0, Counted<CharGraph>::objectsAlive);
    EXPECT_EQ(0, Counted<CharGraph::Node>::objectsAlive);
    EXPECT_EQ(0, Counted<CharGraph::Edge>::objectsAlive);
    EXPECT_EQ(0, Counted<CharDigraph>::objectsAlive);
    EXPECT_EQ(0, Counted<CharDigraph::Node>::objectsAlive);
    EXPECT_EQ(0, Counted<CharDigraph::Edge>::objectsAlive);
}

TEST(P80, GraphCanBeCreatedFromTermsAndAdjacencyList) {
    auto graph = CharGraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    delete(graph);

    graph = CharGraph::adjacent({
            CharAdjacency('a', {'b'}),
            CharAdjacency('b', {'c'}),
            CharAdjacency('c', {})
    });
    delete(graph);

    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, DirectedGraphCanBeCreatedFromTermsAndAdjacencyList) {
    auto graph = CharDigraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    delete(graph);

    graph = CharDigraph::adjacent({
            CharAdjacency('a', {'b'}),
            CharAdjacency('b', {'c'}),
            CharAdjacency('c', {})
    });
    delete(graph);

    expectAllGraphObjectsToBeDeleted();
}
