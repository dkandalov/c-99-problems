#include <gtest/gtest.h>
#include "p99-graph.cpp"

using CharGraph = Graph<char, int>;
using CharDigraph = Digraph<char, int>;
using CharTuple = Tuple<char, char>;
using CharTuple3 = Tuple3<char, char, int>;
using CharAdjacency = Tuple<char, Vector<char>>;
using CharAdjacencyLabeled = Tuple<char, Vector<Tuple<char, int>>>;

template<typename T, typename U>
void expectEqualGraphs(p<Graph<T, U>>& expected, p<Graph<T, U>>& actual) {
    std::cout << "expected graph: " << expected->toString() << "\n";
    std::cout << "actual graph:   " << actual->toString() << "\n";
    std::flush(std::cout);
    EXPECT_TRUE(expected->equalTo(actual.get()));
    std::cout << "-----------" << "\n";
}

template<typename T, typename U>
void expectNotEqualGraphs(p<Graph<T, U>>& expected, p<Graph<T, U>>& actual) {
    std::cout << "expected graph: " << expected->toString() << "\n";
    std::cout << "actual graph:   " << actual->toString() << "\n";
    std::flush(std::cout);
    EXPECT_FALSE(expected->equalTo(actual.get()));
    std::cout << "-----------" << "\n";
}

template<typename T, typename U>
void expectEqualGraphVectors(Vector<p<Graph<T, U>>>& expected,
                             Vector<p<Graph<T, U>>>& actual) {
    EXPECT_EQ(expected.size(), actual.size());
    for (auto i = expected.begin(), j = actual.begin();
         i != expected.end() && j != actual.end(); i++, j++) {
        expectEqualGraphs(*i, *j);
    }
}

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

class GraphTest : public ::testing::Test {
protected:
    virtual void TearDown() override {
        expectAllGraphObjectsToBeDeleted();
    }
};


TEST_F(GraphTest, P80_GraphCanBeCreatedFromTermsAndAdjacencyList) {
    auto graph1 = CharGraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    auto graph2 = CharGraph::adjacent({
            CharAdjacency('a', {'b'}),
            CharAdjacency('b', {'c'}),
            CharAdjacency('c', {})
    });
}

TEST_F(GraphTest, P80_DirectedGraphCanBeCreatedFromTermsAndAdjacencyList) {
    auto graph1 = CharDigraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    auto graph2 = CharDigraph::adjacent({
            CharAdjacency('a', {'b'}),
            CharAdjacency('b', {'c'}),
            CharAdjacency('c', {})
    });
}

TEST_F(GraphTest, P80_GraphEquality) {
    auto graph1 = CharGraph::term(
            {'a', 'b', 'c'},
            {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    auto graph2 = CharGraph::term(
            {'a', 'b', 'c'},
            {CharTuple('a', 'b'), CharTuple('c', 'b')}
    );
    expectEqualGraphs(graph1, graph1);
    expectEqualGraphs(graph1, graph2);
    expectEqualGraphs(graph2, graph2);

    auto graph3 = CharGraph::term(
        {'a', 'b', 'c', 'd'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    expectNotEqualGraphs(graph1, graph3);

    auto graph4 = CharGraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c'), CharTuple{'c', 'a'}}
    );
    expectNotEqualGraphs(graph1, graph4);
}

TEST_F(GraphTest, P80_GraphConverstionToTermForm) {
    auto graph = CharGraph::term(
        {'a', 'b', 'c'},
        { CharTuple('a', 'b'), CharTuple('b', 'c') }
    );
    Vector<char> nodeValues = {'a', 'b', 'c'};
    Vector<CharTuple3> edges = { CharTuple3('a', 'b', 0), CharTuple3('b', 'c', 0) };
    auto expected = std::make_tuple(nodeValues, edges);

    EXPECT_EQ(expected, graph->toTermForm());
}

TEST_F(GraphTest, P80_GraphConverstionToAdjacentForm) {
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
}

TEST_F(GraphTest, P80_DigraphConverstionToAdjacentForm) {
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
}

TEST_F(GraphTest, P80_GraphToString) {
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
}

TEST_F(GraphTest, P80_DigraphToString) {
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
}

TEST_F(GraphTest, P80_GraphFromString) {
    auto graph = CharGraph::fromString("[b-c, f-c, g-h, d, f-b, k-f, h-g]");
    EXPECT_EQ("[b-c, d, f-c, f-b, g-h, k-f]", graph->toString());

    auto labeledGraph = CharGraph::fromString("[b-c/1, f-c/2, g-h/3, d, f-b/4, k-f/5, h-g/6]");
    EXPECT_EQ("[b-c/1, d, f-c/2, f-b/4, g-h/3, k-f/5]", labeledGraph->toString());
}

TEST_F(GraphTest, P80_DigraphFromString) {
    auto graph = CharDigraph::fromString("[b>c, f>c, g>h, d, f>b, k>f, h>g]");
    EXPECT_EQ("[b>c, c, d, f>c, f>b, g>h, h>g, k>f]", graph->toString());

    auto labeledGraph = CharDigraph::fromString("[b>c/1, f>c/2, g>h/3, d, f>b/4, k>f/5, h>g/6]");
    EXPECT_EQ("[b>c/1, c, d, f>c/2, f>b/4, g>h/3, h>g/6, k>f/5]", labeledGraph->toString());
}

TEST_F(GraphTest, P81_FindAllPathsFromOneNodeToAnother_InDigraph) {
    auto graph = CharDigraph::fromString("[p>q/9, m>q/7, k, p>m/5]");

    Vector<Vector<char>> expected = {{'p', 'q'}, {'p', 'm', 'q'}};
    EXPECT_EQ(expected, graph->findPaths('p', 'q'));

    expected = {{'m', 'q'}};
    EXPECT_EQ(expected, graph->findPaths('m', 'q'));

    expected = {};
    EXPECT_EQ(expected, graph->findPaths('q', 'p'));
}

TEST_F(GraphTest, P81_FindAllPathsFromOneNodeToAnother_InGraph) {
    auto graph = CharGraph::fromString("[b-c, f-c, g-h, d, f-b, k-f, h-g]");

    Vector<Vector<char>> expected = {};
    EXPECT_EQ(expected, graph->findPaths('g', 'd'));

    expected = {{'g', 'h'}};
    EXPECT_EQ(expected, graph->findPaths('g', 'h'));

    expected = {{'k', 'f', 'c'}, {'k', 'f', 'b', 'c'}};
    EXPECT_EQ(expected, graph->findPaths('k', 'c'));
}

TEST_F(GraphTest, P82_FindCycleFromNode) {
    auto graph = CharGraph::fromString("[b-c, f-c, g-h, d, f-b, k-f, h-g]");

    Vector<Vector<char>> expected = {{'f', 'c', 'b', 'f'}, {'f', 'b', 'c', 'f'}};
    EXPECT_EQ(expected, graph->findCycles('f'));
}

TEST_F(GraphTest, P83_ConstructAllSpanningTrees) {
    auto graph = CharGraph::fromString("[a-b, b-c, a-c]");
    Vector<p<CharGraph>> actual = graph->allSpanningTrees();

    Vector<p<CharGraph>> expected;
    expected.push_back(CharGraph::fromString("[a-b, b-c]"));
    expected.push_back(CharGraph::fromString("[a-c, c-b]"));
    expected.push_back(CharGraph::fromString("[a-b, a-c]"));
    expectEqualGraphVectors(expected, actual);


    graph = CharGraph::fromString("[a-b, b-c, c-d, d-a]");
    actual = graph->allSpanningTrees();

    expected.clear();
    expected.push_back(CharGraph::fromString("[a-b, b-c, c-d]"));
    expected.push_back(CharGraph::fromString("[a-d, c-b, d-c]"));
    expected.push_back(CharGraph::fromString("[a-d, b-a, d-c]"));
    expected.push_back(CharGraph::fromString("[a-d, b-a, c-b]"));
    expectEqualGraphVectors(expected, actual);
}

TEST_F(GraphTest, P83_ConstructAllSpanningTrees_ComplexExample) {
    auto graph = CharGraph::fromString("[a-b, a-d, b-c, b-e, c-e, d-e, d-f, d-g, e-h, f-g, g-h]");
    Vector<p<CharGraph>> actual = graph->allSpanningTrees();
    EXPECT_EQ(14, actual.size());
}

TEST_F(GraphTest, P84_ConstructMinimalSpanningTree) {
    auto graph = CharGraph::fromString("[a-b/1, b-c/2, a-c/3]");
    auto expected = CharGraph::fromString("[a-b/1, b-c/2]");
    auto actual = graph->minimalSpanningTree();
    expectEqualGraphs(expected, actual);
}

TEST_F(GraphTest, P84_ConstructMinimalSpanningTree_ComplexExample) {
    auto graph = CharGraph::fromString(
            "[a-b/5, a-d/3, b-c/2, b-e/5, c-e/6, e-d/7, e-h/5, h-g/1, g-d/3, g-f/4, f-d/4]"
    );
    auto expected = CharGraph::fromString(
            "[a-d/3, b-e/5, b-c/2, e-h/5, g-d/3, g-f/4, h-g/1]"
    );
    auto actual = graph->minimalSpanningTree();
    expectEqualGraphs(expected, actual);
}

TEST_F(GraphTest, P85_GraphIsomorphism) {
    auto graph1 = CharGraph::fromString("[a-b]");
    auto graph2 = CharGraph::fromString("[A-B]");
    EXPECT_TRUE(graph1->isIsomorphicTo(graph2));

    graph1 = CharGraph::fromString("[a-b, b-c, c-a]");
    graph2 = CharGraph::fromString("[A-B, B-C, C-A]");
    EXPECT_TRUE(graph1->isIsomorphicTo(graph2));
}

TEST_F(GraphTest, P86_GraphNodeDegree) {
    auto graph = CharGraph::fromString("[a-b, b-c, a-c, a-d]");
    EXPECT_EQ(3, graph->nodeDegreeOf('a'));
    EXPECT_EQ(2, graph->nodeDegreeOf('b'));
    EXPECT_EQ(2, graph->nodeDegreeOf('c'));
    EXPECT_EQ(1, graph->nodeDegreeOf('d'));

    EXPECT_EQ({'a', 'b', 'c', 'd'}, graph->nodesByDegree());
}