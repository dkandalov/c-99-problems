#include <gtest/gtest.h>
#include "p99-graph.cpp"

using CharGraph = Graph<char, char>;
using CharTuple = Tuple<char>;

TEST(P80, CreatingGraphFromTerms) {
    auto graph = CharGraph::term(
        {'a', 'b', 'c'},
        {CharTuple('a', 'b'), CharTuple('b', 'c')}
    );
    // TODO check object counts
    delete(graph);
}