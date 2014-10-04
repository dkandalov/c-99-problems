#include <iostream>
#include <list>

using namespace std;

int main() {
    std::list<int> list = {1, 1, 2, 3, 5, 8};
    int lastElement = list.back();

    cout << lastElement << endl;

    return 0;
}