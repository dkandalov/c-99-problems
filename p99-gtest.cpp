#include "p99.cpp"
#include "lib/gtest-1.7.0/include/gtest/gtest.h"

TEST(P1, LastElementOfList) {
    EXPECT_EQ(8, lastElementOf((List<int>) {1, 1, 2, 3, 5, 8}));
}

TEST(P2, PenultimateElementOfList) {
    EXPECT_THROW(penultimate((List<int>) {}), std::invalid_argument);
    EXPECT_THROW(penultimate((List<int>) {1}), std::invalid_argument);

    EXPECT_EQ(1, penultimate((List<int>) {1, 2}));
    EXPECT_EQ(5, penultimate((List<int>) {1, 1, 2, 3, 5, 8}));
}

TEST(P3, FindNthElementOfList) {
    List<int> list = {1, 1, 2, 3, 5, 8};
    EXPECT_EQ(1, getElement(0, list));
    EXPECT_EQ(1, getElement(1, list));
    EXPECT_EQ(2, getElement(2, list));
    EXPECT_EQ(3, getElement(3, list));
    EXPECT_EQ(5, getElement(4, list));
    EXPECT_EQ(8, getElement(5, list));
}

TEST(P4, NumberOfElementsInList) {
    EXPECT_EQ(0, sizeOf((List<int>) {}));
    EXPECT_EQ(1, sizeOf((List<int>) {1}));
    EXPECT_EQ(6, sizeOf((List<int>) {1, 1, 2, 3, 5, 8}));
}

TEST(P5, ReverseList) {
    List<int> expected = {};
    EXPECT_EQ(expected, reverse((List<int>) {}));

    expected = {1};
    EXPECT_EQ(expected, reverse((List<int>) {1}));

    expected = {8, 5, 3, 2, 1, 1};
    EXPECT_EQ(expected, reverse((List<int>) {1, 1, 2, 3, 5, 8}));
}

TEST(P6, IsListAPalindrome) {
    EXPECT_EQ(true, isPalindrome((List<int>) {}));
    EXPECT_EQ(true, isPalindrome((List<int>) {1}));
    EXPECT_EQ(true, isPalindrome((List<int>) {1, 2, 3, 2, 1}));
    EXPECT_EQ(false, isPalindrome((List<int>) {1, 1, 2, 3, 5, 8}));
}

TEST(P7, FlattenList) {
    EXPECT_EQ((List<int>) {}, flatten((List<List<int>>) {}));
    EXPECT_EQ((List<int>) {1}, flatten((List<List<int>>) {{1}}));
    List<int> expected = {1, 2, 3, 4};
    EXPECT_EQ(expected, flatten((List<List<int>>) {{1, 2}, {3, 4}}));
}

TEST(P8, CompressList) {
    EXPECT_EQ((List<int>) {}, compress((List<int>) {}));
    EXPECT_EQ((List<int>) {1}, compress((List<int>) {1}));
    EXPECT_EQ((List<int>) {1}, compress((List<int>) {1, 1}));
    
    List<char> expected = {'a', 'b', 'c', 'a', 'd', 'e'};
    List<char> actual = compress((List<char>){'a', 'a', 'a', 'a', 'b', 'c', 'c', 'a', 'a', 'd', 'e', 'e', 'e', 'e'});
    EXPECT_EQ(expected, actual);
}

TEST(P9, PackList) {
    EXPECT_EQ((List<List<int>>) {}, pack((List<int>) {}));
    EXPECT_EQ((List<List<int>>) {{1}}, pack((List<int>) {1}));
    
    List<List<char>> expected = {{'a', 'a'}};
    List<List<char>> actual = pack((List<char>) {'a', 'a'});
    EXPECT_EQ(expected, actual);

    expected = {{'a', 'a', 'a', 'a'}, {'b'}, {'c', 'c'}, {'a', 'a'}, {'d'}, {'e', 'e', 'e', 'e'}};
    actual = pack((List<char>) {'a', 'a', 'a', 'a', 'b', 'c', 'c', 'a', 'a', 'd', 'e', 'e', 'e', 'e'});
    EXPECT_EQ(expected, actual);
}

TEST(P10, RunLengthEncodingOfAList) {
    EXPECT_EQ((List<Tuple<int, int>>) {}, encode((List<int>) {}));

    List<Tuple<int, char>> expected = {pair(1, 'a')};
    List<Tuple<int, char>> actual = encode((List<char>) {'a'});
    EXPECT_EQ(expected, actual);

    expected = {pair(2, 'a')};
    actual = encode((List<char>) {'a', 'a'});
    EXPECT_EQ(expected, actual);
}

TEST(P11, ModifiedRunLengthEncodingOfAList) {
    List<Either<Tuple<int, char>, char>> actual = encodeModified((List<char>) {});
    List<Either<Tuple<int, char>, char>> expected = (List<Either<Tuple<int, char>, char>>) {};
    EXPECT_EQ(expected, actual);

    actual = encodeModified((List<char>) {'a', 'a', 'b'});
    expected = (List<Either<Tuple<int, char>, char>>) {
            Left<Tuple<int, char>>(pair(2, 'a')),
            Right<char>('b')
    };
    EXPECT_EQ(expected, actual);
}

TEST(P12, DecodeRunLengthEncodedAList) {
    EXPECT_EQ((List<char>) {}, decode((List<Tuple<int, char>>) {}));
    EXPECT_EQ((List<char>) {'a'}, decode((List<Tuple<int, char>>) { pair(1, 'a') }));
}

TEST(P13, RunLengthEncodingOfAList) {
    EXPECT_EQ((List<Tuple<int, int>>) {}, encodeDirect((List<int>) {}));

    List<Tuple<int, char>> expected = {pair(1, 'a')};
    List<Tuple<int, char>> actual = encodeDirect((List<char>) {'a'});
    EXPECT_EQ(expected, actual);

    expected = {pair(2, 'a')};
    actual = encodeDirect((List<char>) {'a', 'a'});
    EXPECT_EQ(expected, actual);

    expected = {pair(2, 'a'), pair(1, 'b')};
    actual = encodeDirect((List<char>) {'a', 'a', 'b'});
    EXPECT_EQ(expected, actual);
}

TEST(P14, DuplicateElementsOfAList) {
    EXPECT_EQ((List<char>) {}, duplicate((List<char>) {}));

    List<char> expected = {'a', 'a'};
    List<char> actual = duplicate((List<char>) { 'a' });
    EXPECT_EQ(expected, actual);

    expected = {'a', 'a', 'b', 'b', 'c', 'c', 'c', 'c'};
    actual = duplicate((List<char>) { 'a', 'b', 'c', 'c' });
    EXPECT_EQ(expected, actual);
}

TEST(P15, DuplicateNElementsOfAList) {
    List<char> expected = {'a', 'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c', 'c', 'c', 'c'};
    List<char> actual = duplicateN(3, (List<char>) { 'a', 'b', 'c', 'c' });
    EXPECT_EQ(expected, actual);
}

TEST(P16, DropEveryNthElementFromList) {
    List<int> expected = {1, 2, 4, 5, 7, 8, 10};
    List<int> actual = dropEvery(3, (List<int>) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    EXPECT_EQ(expected, actual);
}

TEST(P17, SplitList) {
    List<int> part1 = {1, 2, 3};
    List<int> part2 = {4, 5, 6, 7, 8, 9, 10};
    Tuple<List<int>, List<int>> expected = pair(part1, part2);
    Tuple<List<int>, List<int>> actual = split(3, (List<int>) { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    EXPECT_EQ(expected, actual);
}

TEST(P18, ExtractSliceFromAList) {
    List<int> expected = { 4, 5, 6, 7 };
    List<int> actual = slice(3, 7, (List<int>) { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    EXPECT_EQ(expected, actual);
}

TEST(P19, RotateListNPlaces) {
    List<int> expected = {4, 5, 6, 7, 8, 9, 10, 1, 2, 3};
    List<int> actual = rotate(3, (List<int>) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    EXPECT_EQ(expected, actual);

    expected = {9, 10, 1, 2, 3, 4, 5, 6, 7, 8};
    actual = rotate(-2, (List<int>) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    EXPECT_EQ(expected, actual);
}

TEST(P20, RemoveNthElementFromList) {
    Tuple<List<int>, int> expected = pair((List<int>) {1, 2, 3, 5, 6, 7, 8, 9, 10}, 4);
    Tuple<List<int>, int> actual = removeAt(3, (List<int>) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    EXPECT_EQ(expected, actual);
}

TEST(P21, InsertElementIntoList) {
    List<int> expected = {1, 2, 3, 123, 4, 5, 6, 7, 8, 9, 10};
    List<int> actual = insertAt(3, 123, (List<int>) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    EXPECT_EQ(expected, actual);
}

TEST(P22, CreateListWithIntegersFromRange) {
    List<int> expected = {4, 5, 6, 7, 8, 9};
    List<int> actual = range(4, 9);
    EXPECT_EQ(expected, actual);
}

TEST(P23, ExtractGivenNumberOfRandomlySelectedElements) {
    unsigned int seed = 123;
    List<int> expected = {2, 10, 9};
    List<int> actual = randomSelect(seed, 3, (List<int>) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    EXPECT_EQ(expected, actual);

    List<int> list = randomSelect(5, (List<int>) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    EXPECT_EQ(5, sizeOf(list));
}

TEST(P24, Lotto_DrawNDifferentRandomNumbersFromRange1ToM) {
    unsigned int seed = 123;
    List<int> expected = {1, 13, 20, 8, 45};
    List<int> actual = lotto(seed, 5, 49);
    EXPECT_EQ(expected, actual);

    List<int> list = lotto(5, 50);
    EXPECT_EQ(5, sizeOf(list));
}

TEST(P25, GenerateRandomPermutationOfAList) {
    unsigned int seed = 123;
    List<int> expected = {2, 5, 3, 1, 4};
    List<int> actual = randomPermute(seed, (List<int>) {1, 2, 3, 4, 5});
    EXPECT_EQ(expected, actual);

    List<int> list = randomPermute(range(1, 100));
    EXPECT_EQ(100, sizeOf(list));
}

TEST(P26, Combinations) {
    List<List<int>> expected = {{1}, {2}, {3}};
    List<List<int>> actual = combinations(1, (List<int>) {1, 2, 3});
    EXPECT_EQ(expected, actual);

    expected = {{1, 2}, {1, 3}, {2, 3}};
    actual = combinations(2, (List<int>) {1, 2, 3});
    EXPECT_EQ(expected, actual);

    expected = {{1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4}};
    actual = combinations(3, (List<int>) {1, 2, 3, 4});
    EXPECT_EQ(expected, actual);

    actual = combinations(3, (List<int>) range(1, 12));
    EXPECT_EQ(220, sizeOf(actual));
}

TEST(P27, GroupElementsIntoDisjoinedSubsets) {
    List<Combinations<int>> expected = {{{1}, {2, 3}}, {{2}, {1, 3}}, {{3}, {1, 2}}};
    List<Combinations<int>> actual = group((List<int>){1, 2}, (List<int>) range(1, 3));
    EXPECT_EQ(expected, actual);
}

TEST(P28a, SortListsByLengthOfSublists) {
    List<List<int>> expected = {{0}, {1}, {2, 3}, {4, 5, 6}};
    List<List<int>> actual = sortByLength((List<List<int>>) {{2,3}, {1}, {4,5,6}, {0}});
    EXPECT_EQ(expected, actual);
}

TEST(P28b, SortListsByLengthOfSublists) {
    List<List<int>> expected = {{2, 3}, {4, 5, 6}, {1}, {0}};
    List<List<int>> actual = sortByLengthFrequency((List<List<int>>) {{2,3}, {1}, {4,5,6}, {0}});
    EXPECT_EQ(expected, actual);
}

TEST(P31, DetermineIfIntegerIsPrime) {
    EXPECT_EQ(false, isPrime(0));
    EXPECT_EQ(false, isPrime(1));
    EXPECT_EQ(true, isPrime(2));
    EXPECT_EQ(true, isPrime(3));
    EXPECT_EQ(false, isPrime(4));
    EXPECT_EQ(true, isPrime(5));
}

TEST(P32, DetermineGreatestCommonDivisor) {
    EXPECT_EQ(1, gcd(3, 4));
    EXPECT_EQ(9, gcd(36, 63));
}

TEST(P33, DetermineIfNumbersAreCoprime) {
    EXPECT_TRUE(areCoprime(3, 4));
    EXPECT_FALSE(areCoprime(36, 63));
}

TEST(P34, CalculateTotientFunction) {
    EXPECT_EQ(4, totient(10));
    EXPECT_EQ(40, totient(100));
}

TEST(P35, FindPrimeFactorOfNumber) {
    List<int> expected;
    expected = {1};
    EXPECT_EQ(expected, primeFactorsOf(1));
    expected = {2};
    EXPECT_EQ(expected, primeFactorsOf(2));
    expected = {3};
    EXPECT_EQ(expected, primeFactorsOf(3));
    expected = {2, 2};
    EXPECT_EQ(expected, primeFactorsOf(4));
    expected = {3, 3, 5, 7};
    EXPECT_EQ(expected, primeFactorsOf(315));
}

TEST(P36, FindPrimeFactorOfNumberAndTheirMultiplicity) {
    std::unordered_map<int, int> expected = {{3, 2}, {5, 1}, {7, 1}};
    EXPECT_EQ(expected, primeFactorMultiplicityOf(315));
}

TEST(P37, CalculateTotientFunctionImproved) {
    EXPECT_EQ(4, fastTotient(10));
    EXPECT_EQ(40, fastTotient(100));
    EXPECT_EQ(400, fastTotient(1000));
}

TEST(P39, ListOfPrimeNumbers) {
    List<int> expected = {7, 11, 13, 17, 19, 23, 29, 31};
    EXPECT_EQ(expected, listPrimesInRange(7, 31));
}

TEST(P40, GoldbachConjecture) {
    EXPECT_EQ(pair(5, 23), goldbachNumberOf(28));
    EXPECT_EQ(pair(2, 53), goldbachNumberOf(55));
    EXPECT_EQ(pair(3, 97), goldbachNumberOf(100));
}

TEST(P41part1, GolbachConjectureList) {
    std::map<int, Tuple<int, int>> expected = {
            {10, pair(3, 7)},
            {12, pair(5, 7)},
            {14, pair(3, 11)},
            {16, pair(3, 13)},
            {18, pair(5, 13)},
            {20, pair(3, 17)}
    };
    EXPECT_EQ(expected, goldbachList(9, 20));
}

TEST(P41part2, GolbachConjectureList) {
    std::map<int, Tuple<int, int>> expected = {
            {992, pair(73, 919)},
            {1382, pair(61, 1321)},
            {1856, pair(67, 1789)},
            {1928, pair(61, 1867)}
    };
    EXPECT_EQ(expected, goldbachListWithThreshold(1, 2000, 50));
}

TEST(P49, GrayCode) {
    List<std::string> expected = {"0", "1"};
    EXPECT_EQ(expected, grayCode(1));

    expected = {"00", "01", "11", "10"};
    EXPECT_EQ(expected, grayCode(2));

    expected = {"000", "001", "011", "010", "110", "111", "101", "100"};
    EXPECT_EQ(expected, grayCode(3));
}

TEST(P50, HuffmanCode) {
    List<Tuple <char, std::string>> expected = {
            pair('a', "0"), pair('b', "101"), pair('c', "100"),
            pair('d', "111"), pair('e', "1101"), pair('f', "1100")
    };
    EXPECT_EQ(expected, huffman((List<Tuple<char, int>>) {
            pair('a', 45), pair('b', 13), pair('c', 12),
            pair('d', 16), pair('e', 9), pair('f', 5)
    }));
}