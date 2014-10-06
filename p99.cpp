#include <list>
#include <iostream>

template <typename T>
T lastElementOf(const std::list<T>& list) {
    return list.back();
}

template <typename T>
T penultimate(const std::list<T>& list) {
    auto iterator = list.end();
    iterator--;
    iterator--;
    return *iterator;
}

template <typename T>
T getElement(int position, const std::list<T>& list) {
    auto iterator = list.begin();
    for (int i = 0; i < position; i++) {
        iterator++;
    }
    return *iterator;
}

template <typename T>
int sizeOf(const std::list<T>& list) {
    int size = 0;
    for (auto it = list.begin(); it != list.end(); it++) {
        size++;
    }
    return size;
}

template <typename T>
std::list<T> reverse(const std::list<T>& list) {
    std::list<T> result = {};
    for (auto item : list) {
        result.push_front(item);
    }
    return result;
}

template <typename T>
bool isPalindrome(const std::list<T>& list) {
    std::list<T> listCopy = list;

    while (listCopy.size() > 1) {
        T first = listCopy.front();
        T last = listCopy.back();
        if (first != last) return false;

        listCopy.pop_front();
        listCopy.pop_back();
    }
    return true;
}
