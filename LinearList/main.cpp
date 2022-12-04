#include <iostream>
#include "LinearList.h"
#include "ArrayList.h"
#include "VectorList.h"
using namespace std;

int main() {
    VectorList<int> vec1(5);
    VectorList<int> vec2(11);
    for (int i = 0; i < vec1.capacity(); ++i) {
        vec1.insert(i, i);
    }
    for (int i = 0; i < 5; ++i) {
        vec2.insert(i, i*i);
    }
    cout << vec1 << endl;
    cout << vec2 << endl;
    vec2.swap(vec1);
    cout << vec1 << endl;
    cout << vec2 << endl;
}