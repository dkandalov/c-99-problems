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
    auto graph1 = CharGraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    auto graph2 = CharGraph::adjacent({
            CharAdjacency('a', {'b'}),
            CharAdjacency('b', {'c'}),
            CharAdjacency('c', {})
    });

    delete(graph1);
    delete(graph2);
    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, DirectedGraphCanBeCreatedFromTermsAndAdjacencyList) {
    auto graph1 = CharDigraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );

    auto graph2 = CharDigraph::adjacent({
            CharAdjacency('a', {'b'}),
            CharAdjacency('b', {'c'}),
            CharAdjacency('c', {})
    });

    delete(graph1);
    delete(graph2);
    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, GraphEquality) {
    auto graph1 = CharGraph::term(
            {'a', 'b', 'c'},
            {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    EXPECT_TRUE((*graph1) == graph1);

    delete(graph1);
    expectAllGraphObjectsToBeDeleted();
}