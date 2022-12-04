#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "myexception.h"

template<class T> class LinearList;

template<class T>
class VectorList : public LinearList<T> {
public:
    VectorList(int InitCapacity = 10);    //constructor（default initial capacity is 10）
    VectorList(const VectorList<T>&); //copy constructor
    ~VectorList() { delete data; }   //destructor

    bool empty() const { return data->empty(); }
    int size() const { return (int)data->size(); }
    T& get(int index);  //non-const version
    const T& get(int index) const;  //const version
    int indexOf(const T& element) const;
    void erase(int index);
    void insert(int index, const T& element);
    void output(std::ostream& out) const;
    int capacity() const { return (int)data->capacity(); }

    T& operator[](int index) { return get(index); }   //non-const version
    const T& operator[](int index) const { return get(index); }   //const version
    bool operator==(const VectorList<T>& right) const { return *data == *right.data; }
    bool operator!=(const VectorList<T>& right) const { return *data != *right.data; }
    bool operator<(const VectorList<T>& right) const { return *data < *right.data; }
    bool operator<=(const VectorList<T>& right) const { return *data <= *right.data; }
    bool operator>(const VectorList<T>& right) const { return *data > *right.data; }
    bool operator>=(const VectorList<T>& right) const { return *data >= *right.data; }

    void trimToSize() { data->shrink_to_fit();}
    void setSize(int newSize) { data->resize(); }
    void push_back(const T& element) { data->push_back(element); }
    void pop_back(const T& element) { data->pop_back(element); }
    void swap(VectorList<T>& vec) { data->swap(*vec.data); }
    void reserve(int capacity) { data->reserve(capacity); }
    void clear() { data->clear(); }
    void half();
    void meld(const VectorList<T>& vec1, const VectorList<T>& vec2);
    void merge(const VectorList<T>& vec1, const VectorList<T>& vec2);
    void split(VectorList<T>& vec1, VectorList<T>& vec2) const;

    typedef typename std::vector<T>::iterator iterator;
    iterator begin() { return data->begin(); }
    iterator end() { return data->end(); }
    T& front() { return get(0); }   //non-const version
    const T& front() const { return get(0); }   //const version
    T& back() { return get(size() - 1); }  //non-const version
    const T& back() const { return get(size() - 1); }  //const version
protected:
    std::vector<T>* data;
    void checkIndex(int index) const;
};

template<class T>
VectorList<T>::VectorList(int InitCapacity) {
    if (InitCapacity < 1) {
        throw illegalParameterValue("Initial capacity should be > 0.");
    }
    data = new std::vector<T>;
    data->reserve(InitCapacity);
}

template<class T>
VectorList<T>::VectorList(const VectorList<T>& vec) {
    data = new std::vector<T>(*vec.data);
}

template<class T>
void VectorList<T>::checkIndex(int index) const {
    if (index < 0 || index >= data->size()) {
        throw illegalParameterValue("Illegal index!");
    }
}

template<class T>
T& VectorList<T>::get(int index) {
    checkIndex(index);
    return (*data)[index];
}

template<class T>
const T& VectorList<T>::get(int index) const {
    checkIndex(index);
    return (*data)[index];
}

template<class T>
void VectorList<T>::erase(int index) {
    checkIndex(index);
    data->erase(data->begin() + index); //传入地址
}

template<class T>
void VectorList<T>::insert(int index, const T& element) {
    if (index < 0 || index > data->size()) {
        throw illegalParameterValue("Illegal index!");
    }
    data->insert(data->begin() + index, element);   //传入地址
}

template<class T>
int VectorList<T>::indexOf(const T& element) const {
    int index = (int)(std::find(data->begin(), data->end(), element) - data->begin());
    if (index == data->size())  return -1;
    return index;
}

template<class T>
void VectorList<T>::output(std::ostream& out) const {
    std::copy(data->begin(), data->end(), std::ostream_iterator<T>(out, " "));
}

template<class T>
std::ostream& operator<<(std::ostream& out, const VectorList<T>& vecList) {
    vecList.output(out);
    return out;
}

template<class T>
void VectorList<T>::half() {
    int size = (data->size() + 1) / 2;
    for (int i = 1; i < size; ++i) {
        (*data)[i] = (*data)[2 * i];
    }
    for (int i = 1; i < size; ++i) {
        data->erase(data->begin() + size, data->end());
    }
}

template<class T>
void VectorList<T>::meld(const VectorList<T>& vec1, const VectorList<T>& vec2) {
    clear();
    if (vec1.empty() && vec2.empty()) return;
    int minSize = std::min(vec1.size(), vec2.size());
    for (int i = 0; i < minSize; ++i) {
        data->push_back(vec1[i]);
        data->push_back(vec2[i]);
    }
    for (int i = minSize; i < vec1.size(); ++i) {
        data->push_back(vec1[i]);
    }
    for (int i = minSize; i < vec2.size(); ++i) {
        data->push_back(vec2[i]);
    }
}

template<class T>
void VectorList<T>::merge(const VectorList<T>& vec1, const VectorList<T>& vec2) {
    clear();
    if (vec1.empty() && vec2.empty()) return;
    for (int i = 1; i < vec1.size(); ++i) {
        if (vec1[i] < vec1[i - 1]) {
            throw illegalParameterValue("VectorList 1 is not in order.");
        }
    }
    for (int i = 1; i < vec2.size(); ++i) {
        if (vec2[i] < vec2[i - 1]) {
            throw illegalParameterValue("VectorList 2 is not in order.");
        }
    }
    int p1 = 0, p2 = 0;
    while (p1 < vec1.size() && p2 < vec2.size()) {
        if (vec1[p1] < vec2[p2]) {
            data->push_back(vec1[p1]);  ++p1;
        }
        else if (vec1[p1] > vec2[p2]) {
            data->push_back(vec2[p2]);  ++p2;
        }
        else {
            data->push_back(vec1[p1]);  ++p1;
            data->push_back(vec2[p2]);  ++p2;
        }
    }
    while (p1 < vec1.size()) {
        data->push_back(vec1[p1]);  ++p1;
    }
    while (p2 < vec2.size()) {
        data->push_back(vec2[p2]);  ++p2;
    }
}

template<class T>
void VectorList<T>::split(VectorList<T>& vec1, VectorList<T>& vec2) const {
    vec1.clear();
    vec2.clear();
    for (int i = 0; i < vec1.size(); ++i) {
        if (i & 1) {
            vec1.push_back((*data)[i]);
        }
        else {
            vec2.push_back((*data)[i]);
        }
    }
}