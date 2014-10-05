#include <list>

int lastElementOf(std::list<int> list) {
    return list.back();
}

int penultimate(std::list<int> list) {
    list.pop_back();
    return lastElementOf(list);
}

int getElement(int position, std::list<int> list) {
    std::list<int>::iterator iterator = list.begin();
    for (int i = 0; i < position; i++) {
        iterator++;
    }
    return *iterator;
}

int sizeOf(std::list<int> list) {
    int size = 0;
    for (std::list<int>::iterator it = list.begin(); it != list.end(); it++) {
        size++;
    }
    return size;
}
