#include <list>
#include <iostream>

template<typename T>
using List = std::list<T>;

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
    List<T> result = {};
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
    List<T> result = {};
    for (auto list : listOfLists) {
        result.merge(list);
    }
    return result;
}

template<typename T>
List<T> compress(const List<T> &list) {
    if (sizeOf(list) < 2) return list;

    List<T> result = {};

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

    List<List<T>> result = {};
    auto it = list.begin();
    auto lastItem = *it;
    auto groupedItems = List<T>{ lastItem };

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

template <typename T>
List<std::tuple<int, T>> encode(const List<T> &list) {
    List<std::tuple<int, T>> result = {};

    auto groupedItems = pack(list);
    for (auto subList : groupedItems) {
        auto tuple = std::make_tuple(sizeOf(subList), lastElementOf(subList));
        result.push_back(tuple);
    }

    return result;
}