#pragma once
#include <iostream>
#include <algorithm>
#include "myexception.h"

template<class T>
class LinearList {  //abstract Class
public:
    virtual ~LinearList() {};   //destructor
    virtual bool empty() const = 0; //check if the list is empty 
    virtual int size() const = 0;   //get the size of the list
    virtual T& get(int index) = 0;
    virtual const T& get(int index) const = 0;
    virtual int indexOf(const T& elem) const = 0;
    virtual void erase(int index) = 0;
    virtual void insert(int index, const T& elem) = 0;
    virtual void output(std::ostream& out) const = 0;
};

template<class T>
void changeLength1D(T*& list, int oldCapacity, int newCapacity) {
    if (newCapacity < 0) {
        throw illegalParameterValue("New length must be >= 0.");
    }

    T* newList = new T[newCapacity];
    int length = std::min(oldCapacity, newCapacity);
    std::copy(list, list + length, newList);
    delete [] list;
    list = newList;
}

template<class T>
void changeLength2D(T**& list, int oldVerticalCapacity, int oldHorizontalCapacity, int newVerticalCapacity, int newHorizontalCapacity) {
    if (newHorizontalCapacity < 0 || newVerticalCapacity < 0) {
        throw illegalParameterValue("New length must be >= 0.");
    }

    T** new2DList = new T * [newVerticalCapacity];
    for (int i = 0; i < newVerticalCapacity; ++i) {
        new2DList[i] = new T[newHorizontalCapacity];
    }

    int horizon = std::min(oldHorizontalCapacity, newHorizontalCapacity);
    int vertical = std::min(oldVerticalCapacity, newVerticalCapacity);
    for (int i = 0; i < vertical; ++i) {
        std::copy(list[i], list[i] + horizon, new2DList[i]);
    }
    for (int i = 0; i < oldVerticalCapacity; ++i) {
        delete[] list[i];
    }
    delete[] list;
    list = new2DList;
}