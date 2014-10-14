#define CATCH_CONFIG_MAIN
#include "lib/catch/catch.hpp"
#include "p99.cpp"

TEST_CASE("Duplicate N elements of a list", "P15") {
    List<char> expected = {'a', 'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c', 'c', 'c', 'c'};
    List<char> actual = duplicateN(3, (List<char>) { 'a', 'b', 'c', 'c', 'c' });
    REQUIRE(expected == actual);
}