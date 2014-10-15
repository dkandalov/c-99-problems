#define CATCH_CONFIG_MAIN
#include "lib/catch/catch.hpp"
#include "p99.cpp"

TEST_CASE("Duplicate N elements of a list", "P15") {
    std::list<char> expected = {'a'};
    std::list<char> actual = { 'a', 'b', 'c', 'c', 'c' };
    REQUIRE(expected == actual);
}