#include <list>
#include <iostream>
#include <unordered_map>
#include "either/either.cpp"

template<typename T>
using List = std::list<T>;

template<typename T>
using Combinations = List<List<T>>;

template<typename T1, typename T2>
using Tuple = std::tuple<T1, T2>;

template<typename T1, typename T2>
std::tuple<T1, T2> pair(T1 value1, T2 value2) {
    return std::make_tuple(value1, value2);
}

template<typename T>
int sizeOf(const List<T> &list);


template<typename T>
T lastElementOf(const List<T> &list) {
    return list.back();
}

template<typename T>
T penultimate(const List<T> &list) {
    if (sizeOf(list) < 2)
        throw std::invalid_argument("List size should be >= 2 but was " + std::to_string(sizeOf(list)));

    auto iterator = list.end();
    iterator--;
    iterator--;
    return *iterator;
}

template<typename T>
T getElement(int position, const List<T> &list) {
    auto iterator = list.begin();
    for (int i = 0; i < position; i++) {
        iterator++;
    }
    return *iterator;
}

template<typename T>
int sizeOf(const List<T> &list) {
    int size = 0;
    for (auto it = list.begin(); it != list.end(); it++) {
        size++;
    }
    return size;
}

template<typename T>
List<T> reverse(const List<T> &list) {
    List<T> result;
    for (auto item : list) {
        result.push_front(item);
    }
    return result;
}

template<typename T>
bool isPalindrome(const List<T> &list) {
    List<T> listCopy = list;

    while (listCopy.size() > 1) {
        T first = listCopy.front();
        T last = listCopy.back();
        if (first != last) return false;

        listCopy.pop_front();
        listCopy.pop_back();
    }
    return true;
}

template<typename T>
List<T> flatten(const List<List<T>> &listOfLists) {
    List<T> result;
    for (auto list : listOfLists) {
        result.merge(list);
    }
    return result;
}

template<typename T>
List<T> compress(const List<T> &list) {
    if (sizeOf(list) < 2) return list;

    List<T> result;

    auto it = list.begin();
    result.push_back(*it);
    T lastItem = *it;

    for (it++; it != list.end(); it++) {
        if (*it != lastItem) {
            result.push_back(*it);
            lastItem = *it;
        }
    }
    return result;
}

template<typename T>
List<List<T>> pack(const List<T> &list) {
    if (list.empty()) return {};

    List<List<T>> result;
    auto it = list.begin();
    auto lastItem = *it;
    auto groupedItems = List<T>{lastItem};

    for (it++; it != list.end(); it++) {
        if (*it == lastItem) {
            groupedItems.push_back(*it);
        } else {
            result.push_back(groupedItems);
            lastItem = *it;
            groupedItems = List<T>{lastItem};
        }
    }
    result.push_back(groupedItems);

    return result;
}

template<typename T>
List<Tuple<int, T>> encode(const List<T> &list) {
    List<Tuple<int, T>> result;

    auto groupedItems = pack(list);
    for (auto subList : groupedItems) {
        auto tuple = pair(sizeOf(subList), lastElementOf(subList));
        result.push_back(tuple);
    }

    return result;
}

template<typename T>
List<Either<Tuple<int, T>, T>> encodeModified(const List<T> &list) {
    List<Either<Tuple<int, T>, T>> result;
    for (auto item : encode(list)) {
        if (std::get<0>(item) == 1) {
            result.push_back(Right<T>(std::get<1>(item)));
        } else {
            result.push_back(Left<Tuple<int, T>>(item));
        }
    }
    return result;
}

template<typename T>
List<T> decode(const List<Tuple<int, T>> &encodedList) {
    List<T> result;
    for (auto item : encodedList) {
        for (int i = 0; i < std::get<0>(item); i++) {
            result.push_back(std::get<1>(item));
        }
    }
    return result;
}

template<typename T>
List<Tuple<int, T>> encodeDirect(const List<T> &list) {
    auto it = list.begin();
    if (it == list.end()) return {};

    T lastItem = *it;
    int count = 1;
    List<Tuple<int, T>> result;
    while (++it != list.end()) {
        auto item = *it;
        if (item == lastItem) {
            count++;
        } else {
            result.push_back(pair(count, lastItem));
            count = 1;
        }
        lastItem = item;
    }
    result.push_back(pair(count, lastItem));

    return result;
}

template<typename T>
List<T> duplicateN(int n, const List<T> &list) {
    List<T> result;
    for (auto item : list) {
        for (int i = 0; i < n; i++) {
            result.push_back(item);
        }
    }
    return result;
}

template<typename T>
List<T> duplicate(const List<T> &list) {
    return duplicateN(2, list);
}

template<typename T>
List<T> dropEvery(int index, const List<T> &list) {
    List<T> result;
    int i = 0;
    for (auto item : list) {
        if (++i % index == 0) continue;
        result.push_back(item);
    }
    return result;
}

template<typename T>
Tuple<List<T>, List<T>> split(int splitIndex, const List<T> &list) {
    List<T> part1;
    List<T> part2;
    int i = 0;
    for (auto item : list) {
        if (++i <= splitIndex) {
            part1.push_back(item);
        } else {
            part2.push_back(item);
        }
    }
    return pair(part1, part2);
}

template<typename T>
List<T> slice(int from, int to, const List<T> &list) {
    return std::get<1>(split(from, std::get<0>(split(to, list))));
}

template<typename T>
List<T> rotate(int shift, const List<T> &list) {
    int index = shift >= 0 ? shift : sizeOf(list) + shift;
    auto splitList = split(index, list);

    List<T> part1 = std::get<0>(splitList);
    List<T> part2 = std::get<1>(splitList);
    part1.splice(part1.begin(), part2);

    return part1;
}

template<typename T>
Tuple<List<T>, T> removeAt(int index, const List<T> &list) {
    if (index < 0 || index >= sizeOf(list))
        throw std::invalid_argument(
                "Cannot remove index: " + std::to_string(index) +
                ", list size: " + std::to_string(sizeOf(list)));

    List<T> newList;
    T value;
    int i = 0;
    for (T item : list) {
        if (i++ == index) {
            value = item;
        } else {
            newList.push_back(item);
        }
    }
    return pair(newList, value);
}

template<typename T>
List<T> insertAt(int index, T element, const List<T> &list) {
    List<T> result;
    int i = 0;
    for (auto item : list) {
        if (i++ == index) {
            result.push_back(element);
        }
        result.push_back(item);
    }
    if (index == sizeOf(result)) {
        result.push_back(element);
    }
    return result;
}

List<int> range(int from, int to) {
    List<int> result;
    for (int i = from; i <= to; i++) {
        result.push_back(i);
    }
    return result;
}

template<typename T>
List<T> randomSelect(unsigned int seed, int amount, const List<T> &list) {
    srand(seed);
    List<T> listCopy = list;
    List<T> result;
    for (int i = 0; i < amount; i++) {
        auto removed = removeAt(rand() % sizeOf(listCopy), listCopy);
        listCopy = std::get<0>(removed);
        T item = std::get<1>(removed);
        result.push_back(item);
    }
    return result;
}

template<typename T>
List<T> randomSelect(int amount, const List<T> &list) {
    return randomSelect((unsigned int) time(NULL), amount, list);
}

List<int> lotto(unsigned int seed, int amount, int endOfRange) {
    return randomSelect(seed, amount, range(1, endOfRange));
}

List<int> lotto(int amount, int endOfRange) {
    return lotto((unsigned int) time(NULL), amount, endOfRange);
}

template<typename T>
List<T> randomPermute(unsigned int seed, const List<T> &list) {
    return randomSelect(seed, sizeOf(list), list);
}

template<typename T>
List<T> randomPermute(const List<T> &list) {
    return randomPermute((unsigned int) time(NULL), list);
}

template<typename T>
List<List<T>> combinations(int size, const List<T> &list) {
    List<List<T>> result;
    if (size <= 1) {
        for (auto item : list) {
            result.push_back({item});
        }
        return result;
    }

    auto subList = list;
    for (auto item : list) {
        subList = slice(1, sizeOf(subList), subList);
        auto subResult = combinations(size - 1, subList);
        for (auto combination : subResult) {
            result.push_back(insertAt(0, item, combination));
        }
    }

    return result;
}

template<typename T>
List<T> tailOf(const List<T> &list) {
    return std::get<0>(removeAt(0, list));
}

template<typename T>
List<Combinations<T>> group(List<int> groupSizes, const List<T> &list) {
    List<Combinations<T>> result;
    if (groupSizes.empty() || list.empty()) {
        result.push_back({});
        return result;
    }

    int groupSize = groupSizes.front();
    auto itemCombinations = combinations(groupSize, list);
    for (auto combination : itemCombinations) {
        List<T> listCopy = list;
        for (auto item : combination) {
            listCopy.remove(item);
        }
        auto subResult = group(tailOf(groupSizes), listCopy);
        for (auto subCombinations : subResult) {
            result.push_back(insertAt(0, combination, subCombinations));
        }
    }

    return result;
}

template<typename T>
List<List<T>> sortByLength(const List<List<T>> &list) {
    List<List<T>> result = list;
    result.sort();
    return result;
}

template<typename T>
List<List<T>> sortByLengthFrequency(const List<List<T>> &list) {
    std::unordered_map<int, int> frequencyByLength;
    for (auto item : list) {
        int value;
        std::unordered_map<int, int>::iterator it = frequencyByLength.find(sizeOf(item));
        if (it == frequencyByLength.end()) {
            value = 0;
            std::pair <int,int> newItem = std::make_pair<int, int>(sizeOf(item), 0);
            frequencyByLength.insert(newItem);
        } else {
            value = (*it).second;
        }
        frequencyByLength[sizeOf(item)] = value + 1;
    }

    List<List<T>> result = list;
    result.sort([&](const List<T> &list1, const List<T> &list2) -> bool {
        return frequencyByLength[sizeOf(list1)] < frequencyByLength[sizeOf(list2)];
    });
    return result;
}

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = (int) sqrt(n); i > 1; i--) {
        if (n % i == 0) return false;
    }
    return true;
}

int gcd(int a, int b) {
    if (b < a) {
        int tmp = a;
        a = b;
        b = tmp;
    }
    while (b % a != 0) {
        int reminder = b % a;
        b = a;
        a = reminder;
    }
    return a;
}

bool areCoprime(int a, int b) {
    return gcd(a, b) == 1;
}

int totient(int n) {
    int result = 0;
    for (int i = 1; i <= n; i++) {
        if (areCoprime(i, n)) result++;
    }
    return result;
}

List<int> primeFactorsOf(int n) {
    if (n == 1) return {1};
    List<int> result;
    while (n != 1) {
        for (int factor = 2; factor <= n; factor++) {
            if (isPrime(factor) && n % factor == 0) {
                result.push_back(factor);
                n = n / factor;
                break;
            }
        }
    }
    return result;
}

std::unordered_map<int, int> primeFactorMultiplicityOf(int n) {
    std::unordered_map<int, int> result;
    for (auto factor : primeFactorsOf(n)) {
//        std::unordered_map<int, int>::const_iterator it = result.find(factor);
//        if (it == result.end()) {
//            result.emplace(factor, 1);
//        } else {
            result[factor] = result[factor] + 1;
//        }
    }
    return result;
}


int fastTotient(int n) {
    int result = 1;
    auto primesMultiplicity = primeFactorMultiplicityOf(n);
    for (auto& entry : primesMultiplicity) {
        int prime = entry.first;
        int multiplicity = entry.second;
        result *= (prime - 1) * pow(prime, multiplicity - 1);
    }
    return result;
}

List<int> listPrimesInRange(int from, int to) {
    List<int> result;
    for (auto n : range(from, to)) {
        if (isPrime(n)) result.push_back(n);
    }
    return result;
}

Tuple<int, int> goldbachNumberOf(int n) {
    auto primes = listPrimesInRange(2, n);
    while (!primes.empty()) {
        int prime1 = primes.front();
        primes.pop_front();
        for (auto prime2 :primes) {
            if (prime1 + prime2 == n) {
                return pair(prime1, prime2);
            }
        }
    }
    throw new std::runtime_error("");
}
