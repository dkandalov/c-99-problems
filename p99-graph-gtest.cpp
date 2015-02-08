#include <gtest/gtest.h>
#include "p99-graph.cpp"

using CharGraph = Graph<char, int>;
using CharDigraph = Digraph<char, int>;
using CharTuple = Tuple<char, char>;
using CharTuple3 = Tuple3<char, char, int>;
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
    auto graph2 = CharGraph::term(
            {'a', 'b', 'c'},
            {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    EXPECT_TRUE((*graph1) == graph1);
    EXPECT_TRUE((*graph1) == graph2);
    EXPECT_TRUE((*graph2) == graph1);

    auto graph3 = CharGraph::term(
        {'a', 'b', 'c', 'd'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    EXPECT_TRUE((*graph1) != graph3);

    auto graph4 = CharGraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c'), CharTuple{'c', 'a'}}
    );
    EXPECT_TRUE((*graph1) != graph4);

    delete(graph1);
    delete(graph2);
    delete(graph3);
    delete(graph4);
    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, ConverstionToTermForm) {
    auto graph1 = CharGraph::term(
        {'a', 'b', 'c'},
        { CharTuple('a', 'b'), CharTuple('b', 'c') }
    );
    Vector<char> nodeValues = {'a', 'b', 'c'};
    Vector<CharTuple3> edges = { CharTuple3('a', 'b', 0), CharTuple3('b', 'c', 0) };
    Tuple<Vector<char>, Vector<CharTuple3>> expected = std::make_tuple(
            nodeValues,
            edges
    );
    EXPECT_EQ(expected, graph1->toTermForm());

    delete(graph1);
}