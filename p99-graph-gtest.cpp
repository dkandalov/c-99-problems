#include <gtest/gtest.h>
#include "p99-graph.cpp"

using CharGraph = Graph<char, char>;
using CharTuple = Tuple<char, char>;

void expectAllGraphObjectsToBeDeleted() {
    std::cout << "graphs created: " << Counted<CharGraph>::objectsCreated << "\n";
    std::cout << "nodes created: " << Counted<CharGraph::Node>::objectsCreated << "\n";
    std::cout << "edges created: " << Counted<CharGraph::Edge>::objectsCreated << "\n";

    EXPECT_EQ(0, Counted<CharGraph>::objectsAlive);
    EXPECT_EQ(0, Counted<CharGraph::Node>::objectsAlive);
    EXPECT_EQ(0, Counted<CharGraph::Edge>::objectsAlive);
}

TEST(P80, GraphCanBeCreatedFromTermsAndAdjacencyList) {
    auto graph = CharGraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    delete(graph);

    graph = CharGraph::adjacent({
        Tuple<char, Vector<char>>('a', {'b'})
    });
    delete(graph);

    expectAllGraphObjectsToBeDeleted();
}