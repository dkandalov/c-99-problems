#include <gtest/gtest.h>
#include "p99-graph.cpp"

using CharGraph = Graph<char, int>;
using CharDigraph = Digraph<char, int>;
using CharTuple = Tuple<char, char>;
using CharTuple3 = Tuple3<char, char, int>;
using CharAdjacency = Tuple<char, Vector<char>>;
using CharAdjacencyLabeled = Tuple<char, Vector<Tuple<char, int>>>;

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

TEST(P80, GraphConverstionToTermForm) {
    auto graph = CharGraph::term(
        {'a', 'b', 'c'},
        { CharTuple('a', 'b'), CharTuple('b', 'c') }
    );
    Vector<char> nodeValues = {'a', 'b', 'c'};
    Vector<CharTuple3> edges = { CharTuple3('a', 'b', 0), CharTuple3('b', 'c', 0) };
    auto expected = std::make_tuple(nodeValues, edges);

    EXPECT_EQ(expected, graph->toTermForm());

    delete(graph);
    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, GraphConverstionToAdjacentForm) {
    auto graph = CharGraph::term(
        {'a', 'b', 'c'},
        { CharTuple('a', 'b'), CharTuple('b', 'c') }
    );
    Vector<CharAdjacencyLabeled> expected = {
            CharAdjacencyLabeled('a', {Tuple<char, int>('b', 0)}),
            CharAdjacencyLabeled('b', {Tuple<char, int>('a', 0), Tuple<char, int>('c', 0)}),
            CharAdjacencyLabeled('c', {Tuple<char, int>('b', 0)})
    };

    EXPECT_EQ(expected, graph->toAdjacentForm());

    delete(graph);
    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, DigraphConverstionToAdjacentForm) {
    auto digraph = CharDigraph::term(
        {'a', 'b', 'c'},
        { CharTuple('a', 'b'), CharTuple('b', 'c') }
    );
    Vector<CharAdjacencyLabeled> expected = {
            CharAdjacencyLabeled('a', {Tuple<char, int>('b', 0)}),
            CharAdjacencyLabeled('b', {Tuple<char, int>('c', 0)}),
            CharAdjacencyLabeled('c', {})
    };

    EXPECT_EQ(expected, digraph->toAdjacentForm());

    delete(digraph);
    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, GraphToString) {
    auto graph = CharGraph::term(
        {'a', 'b', 'c'},
        { CharTuple('a', 'b'), CharTuple('b', 'c') }
    );
    EXPECT_EQ("[a-b, b-c]", graph->toString());

    auto labeledGraph = CharGraph::termLabel(
        {'a', 'b', 'c'},
        { CharTuple3('a', 'b', 1), CharTuple3('b', 'c', 2) }
    );
    EXPECT_EQ("[a-b/1, b-c/2]", labeledGraph->toString());

    delete(graph);
    delete(labeledGraph);
    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, DigraphToString) {
    auto graph = CharDigraph::term(
        {'a', 'b', 'c'},
        { CharTuple('a', 'b'), CharTuple('b', 'a'), CharTuple('b', 'c') }
    );
    EXPECT_EQ("[a>b, b>a, b>c, c]", graph->toString());

    auto labeledGraph = CharDigraph::termLabel(
        {'a', 'b', 'c'},
        { CharTuple3('a', 'b', 1), CharTuple3('b', 'a', 2), CharTuple3('b', 'c', 3) }
    );
    EXPECT_EQ("[a>b/1, b>a/2, b>c/3, c]", labeledGraph->toString());

    delete(graph);
    delete(labeledGraph);
    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, GraphFromString) {
    auto graph = CharGraph::fromString("[b-c, f-c, g-h, d, f-b, k-f, h-g]");
    EXPECT_EQ("[b-c, d, f-c, f-b, g-h, h-g, k-f]", graph->toString());

    auto labeledGraph = CharGraph::fromString("[b-c/1, f-c/2, g-h/3, d, f-b/4, k-f/5, h-g/6]");
    EXPECT_EQ("[b-c/1, d, f-c/2, f-b/4, g-h/3, h-g/6, k-f/5]", labeledGraph->toString());

    delete(graph);
    delete(labeledGraph);
    expectAllGraphObjectsToBeDeleted();
}

TEST(P80, DigraphFromString) {
    auto graph = CharDigraph::fromString("[b>c, f>c, g>h, d, f>b, k>f, h>g]");
    EXPECT_EQ("[b>c, c, d, f>c, f>b, g>h, h>g, k>f]", graph->toString());

    auto labeledGraph = CharDigraph::fromString("[b>c/1, f>c/2, g>h/3, d, f>b/4, k>f/5, h>g/6]");
    EXPECT_EQ("[b>c/1, c, d, f>c/2, f>b/4, g>h/3, h>g/6, k>f/5]", labeledGraph->toString());

    delete(graph);
    delete(labeledGraph);
    expectAllGraphObjectsToBeDeleted();
}