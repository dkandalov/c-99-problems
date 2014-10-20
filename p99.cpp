#include <list>
#include <iostream>
#include "either/either.cpp"

template<typename T>
using List = std::list<T>;

template<typename T1, typename T2>
using Tuple = std::tuple<T1, T2>;

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
        auto tuple = std::make_tuple(sizeOf(subList), lastElementOf(subList));
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
            result.push_back(std::make_tuple(count, lastItem));
            count = 1;
        }
        lastItem = item;
    }
    result.push_back(std::make_tuple(count, lastItem));

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
    return std::make_tuple(part1, part2);
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
    return std::make_tuple(newList, value);
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
