#pragma once
#include <iostream>
#include <algorithm>
#include <iterator>
#include "myexception.h"

template<class T> class LinearList;

template<class T>
class ArrayList : public LinearList<T> {
public:
    ArrayList(int InitCapacity = 10, int capIncrease = 0);    //constructor£¨default initial capacity is 10£©
    ArrayList(const ArrayList<T>&); //copy constructor
    ~ArrayList() { delete[] data; }   //destructor

    bool empty() const { return arrSize == 0; }
    int size() const { return arrSize; }
    T& get(int index);  //non-const version
    const T& get(int index) const; //const version
    int indexOf(const T& element) const;
    void erase(int index);
    void insert(int index, const T& element);
    void output(std::ostream& out) const;
    int capacity() const { return arrCapacity; }

    T& operator[](int index) { return get(index); }   //non-const version
    const T& operator[](int index) const { return get(index); }   //const version
    bool operator==(const ArrayList<T>& right) const;
    bool operator!=(const ArrayList<T>& right) const { return !(*this == right); }
    bool operator<(const ArrayList<T>& right) const;
    bool operator<=(const ArrayList<T>& right) const;
    bool operator>(const ArrayList<T>& right) const { return right < *this; }
    bool operator>=(const ArrayList<T>& right) const { return right <= *this; }

    void trimToSize();
    void setSize(int newSize);
    void push_back(const T& element);
    void pop_back();
    void swap(ArrayList& list);
    void reserve(int capacity);
    void set(int index, const T& element);
    void clear();
    void removeRange(int left, int right);
    int lastIndexOf(const T& element) const;
    void reverse();
    void leftShift(int shift);
    void circularShift(int shift);
    void half();
    void meld(const ArrayList<T>& list1, const ArrayList<T>& list2);
    void merge(const ArrayList<T>& list1, const ArrayList<T>& list2);
    void split(ArrayList<T>& list1, ArrayList<T>& list2) const;

    class iterator;
    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + arrSize); }
    T& front() { return get(0); }   //non-const version
    const T& front() const { return get(0); }   //const version
    T& back() { return get(arrSize - 1); }  //non-const version
    const T& back() const { return get(arrSize - 1); }  //const version
protected:
    void checkIndex(int index) const;

    T* data;
    int arrCapacity;
    int arrSize;
    int initialCapacity;
    int increase;
};

template<class T>
class ArrayList<T>::iterator {
public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;

    iterator(T* pos = 0) { position = pos; }

    T& operator*() const { return *position; }
    T* operator->() const { return &*position; }
    bool operator==(const iterator& right) const { return position == right.position; }
    bool operator!=(const iterator& right) const { return position != right.position; }

    iterator& operator++() { ++position; return *this; }
    iterator operator++(int) {
        iterator original = *this;
        ++position;
        return original;
    }
    iterator& operator--() { --position; return *this; }
    iterator operator--(int) {
        iterator original = *this;
        --position;
        return original;
    }
protected:
    T* position;
};

template<class T>
ArrayList<T>::ArrayList(int InitCapacity, int capIncrease) {
    if (InitCapacity < 1) {
        throw illegalParameterValue("Initial capacity must be > 0.");
    }
    if (capIncrease < 0) {
        throw illegalParameterValue("Capacity increase must be >= 0.");
    }
    arrCapacity = InitCapacity;
    initialCapacity = InitCapacity;
    increase = capIncrease;
    arrSize = 0;
    data = new T[arrCapacity];
}

template<class T>
ArrayList<T>::ArrayList(const ArrayList<T>& arrlist) {
    arrCapacity = arrlist.arrCapacity;
    arrSize = arrlist.arrSize;
    initialCapacity = arrlist.initialCapacity;
    initialCapacity = arrlist.InitCapacity;
    data = new T[arrCapacity];
    std::copy(arrlist.data, arrlist.data + arrSize, data);
}

template<class T>
void ArrayList<T>::checkIndex(int index) const {
    if (index < 0 || index >= arrSize) {
        throw illegalParameterValue("Illegal index!");
    }
}

template<class T>
T& ArrayList<T>::get(int index) {
    checkIndex(index);
    return data[index];
}

template<class T>
const T& ArrayList<T>::get(int index) const {
    checkIndex(index);
    return data[index];
}

template<class T>
int ArrayList<T>::indexOf(const T& element) const {
    int index = (int)(std::find(data, data + arrSize, element) - data);
    if (index == arrSize)   return -1;
    return index;
}

template<class T>
void ArrayList<T>::erase(int index) {
    checkIndex(index);
    std::copy(data + index + 1, data + arrSize, data + index);
    data[--arrSize].~T();
    if (arrSize < arrCapacity / 4) {
        int newLength = std::max(arrCapacity / 2, initialCapacity);
        changeLength1D(data, arrCapacity, newLength);
        arrCapacity = newLength;
    }
}

template<class T>
void ArrayList<T>::insert(int index, const T& element) {
    if (index < 0 || index > arrSize) {
        throw illegalParameterValue("Illegal index!");
    }

    if (arrSize == arrCapacity) {
        int capIncrease = (increase > 0) ? (arrCapacity + increase) : (2 * arrCapacity);
        changeLength1D(data, arrCapacity, capIncrease);
        arrCapacity = capIncrease;
    }
    std::copy_backward(data + index, data + arrSize, data + arrSize + 1);
    data[index] = element;
    ++arrSize;
}

template<class T>
void ArrayList<T>::output(std::ostream& out) const {
    std::copy(data, data + arrSize, std::ostream_iterator<T>(out, " "));
}

template<class T>
std::ostream& operator<<(std::ostream& out, const ArrayList<T>& arrList) {
    arrList.output(out);
    return out;
}

template<class T>
bool ArrayList<T>::operator==(const ArrayList<T>& right) const {
    if (size() != right.size()) return false;
    for (int i = 0; i < size(); ++i) {
        if ((*this)[i] != right[i]) return false;
    }
    return true;
}

template<class T>
bool ArrayList<T>::operator<(const ArrayList<T>& right) const {
    int sz = std::min(size(), right.size());
    for (int i = 0; i < sz; ++i) {
        if ((*this)[i] < right[i]) return true;
        if ((*this)[i] > right[i]) return false;
    }
    return size() < right.size();
}

template<class T>
bool ArrayList<T>::operator<=(const ArrayList<T>& right) const {
    if (*this == right) return true;
    return *this < right;
}

template<class T>
void ArrayList<T>::trimToSize() {
    int length = std::max(arrSize, 1);
    changeLength1D(data, arrCapacity, length);
    arrCapacity = length;
}

template<class T>
void ArrayList<T>::setSize(int newSize) {
    if (arrSize > newSize) {
        for (int i = newSize; i < arrSize; ++i) {
            data[i].~T();
            arrSize = newSize;
        }
    }
}

template<class T>
void ArrayList<T>::push_back(const T& element) {
    if (arrSize == arrCapacity) {
        int capIncrease = (increase > 0) ? (arrCapacity + increase) : (2 * arrCapacity);
        changeLength1D(data, arrCapacity, capIncrease);
        arrCapacity = capIncrease;
    }
    data[arrSize] = element;
    ++arrSize;
}

template<class T>
void ArrayList<T>::pop_back() {
    data[--arrSize].~T();
    if (arrSize < arrCapacity / 4) {
        int newLength = std::max(arrCapacity / 2, initialCapacity);
        changeLength1D(data, arrCapacity, newLength);
        arrCapacity = newLength;
    }
}

template<class T>
void ArrayList<T>::swap(ArrayList& list) {
    std::swap(data, list.data);
    std::swap(arrSize, list.arrSize);
    std::swap(arrCapacity, list.arrCapacity);
}

template<class T>
void ArrayList<T>::reserve(int capacity) {
    if (capacity > arrCapacity) {
        changeLength1D(data, arrCapacity, capacity);
        arrCapacity = capacity;
    }
}

template<class T>
void ArrayList<T>::set(int index, const T& element) {
    checkIndex(index);
    data[index] = element;
}

template<class T>
void ArrayList<T>::clear() {
    T* newData = new T[initialCapacity];
    delete[] data;
    data = newData;
    arrSize = 0;
    arrCapacity = initialCapacity;
}

template<class T>
void ArrayList<T>::removeRange(int left, int right) {
    if (left > right) {
        throw illegalParameterValue("Left index should be < right index.");
    }
    else if (left == right) {
        erase(left);
    }
    else {
        checkIndex(left);
        checkIndex(right);
        std::copy(data + right + 1, data + arrSize, data + left);
        int newSize = arrSize - (right - left + 1);
        for (int i = newSize; i < arrSize; ++i) {
            data[i].~T();
        }
        arrSize = newSize;
        if (arrSize < arrCapacity / 4) {
            int newLength = std::max(arrCapacity / 2, initialCapacity);
            changeLength1D(data, arrCapacity, newLength);
            arrCapacity = newLength;
        }
    }
}

template<class T>
int ArrayList<T>::lastIndexOf(const T& element) const {
    int index;
    for (index = arrSize - 1; index >= 0; --index) {
        if (data[index] == element) break;
    }
    return index;
}

template<class T>
void ArrayList<T>::reverse() {  //member function
    for (int i = 0; i < arrSize / 2; ++i) {
        std::swap(data[i], data[arrSize - i - 1]);
    }
}

template<class T>
void reverse(ArrayList<T>& list) {  //not member function
    int size = list.size();
    for (int i = 0; i < size / 2; ++i) {
        std::swap(list[i], list[size - i - 1]);
    }
}

template<class T>
void ArrayList<T>::leftShift(int shift) {
    if (shift < 0) {
        throw illegalParameterValue("Shifts should be > 0.");
    }
    else if (shift == 0) return;
    else if (shift >= arrSize) {
        clear();
        return;
    }
    else {
        std::copy(data + shift, data + arrSize, data);
        removeRange(arrSize - shift, arrSize - 1);
    }
}

template<class T>
void ArrayList<T>::circularShift(int shift) {
    if (shift < 0) {
        throw illegalParameterValue("Shifts should be > 0.");
    }
    int minShift = shift % arrSize;
    if (minShift == 0) return;

    T* newData = new T[arrCapacity];
    std::copy(data + minShift, data + arrSize, newData);
    std::copy(data, data + minShift, newData + arrSize - minShift);
    delete[] data;
    data = newData;
}

template<class T>
void ArrayList<T>::half() { //member function
    if (arrSize <= 1) return;

    T* newData;
    int newSize = (arrSize + 1) / 2;
    if (newSize < arrCapacity / 4) {
        int newLength = std::max(arrCapacity / 2, initialCapacity);
        newData = new T[newLength];
        arrCapacity = newLength;
    }
    else {
        newData = new T[arrCapacity];
    }

    for (int i = 0; i < newSize; ++i) {
        newData[i] = data[i * 2];
    }
    delete[] data;
    data = newData;
    arrSize = newSize;
}

template<class T>
void half(ArrayList<T>& list) { //not member function
    int newSize = (list.size() + 1) / 2;
    for (int i = 1; i < newSize; ++i) {
        list[i] = list[i * 2];
    }
    list.removeRange(newSize, list.size() - 1);
}

template<class T>
void ArrayList<T>::meld(const ArrayList<T>& list1, const ArrayList<T>& list2) {
    clear();
    if (list1.empty() && list2.empty()) return;
    int minSize = std::min(list1.size(), list2.size());
    for (int i = 0; i < minSize; ++i) {
        push_back(list1[i]);
        push_back(list2[i]);
    }
    for (int i = 0; i < list1.size() - minSize; ++i) {
        push_back(list1[i]);
    }
    for (int i = 0; i < list2.size() - minSize; ++i) {
        push_back(list2[i]);
    }
}

template<class T>
void ArrayList<T>::merge(const ArrayList<T>& list1, const ArrayList<T>& list2) {
    clear();
    if (list1.empty() && list2.empty()) return;
    for (int i = 1; i < list1.size(); ++i) {
        if (list1[i] < list1[i - 1]) {
            throw illegalParameterValue("ArrayList 1 is not in order.");
        }
    }
    for (int i = 1; i < list2.size(); ++i) {
        if (list2[i] < list2[i - 1]) {
            throw illegalParameterValue("ArrayList 2 is not in order.");
        }
    }
    int p1 = 0, p2 = 0;
    while (p1 < list1.size() && p2 < list2.size()) {
        if (list1[p1] < list2[p2]) {
            push_back(list1[p1]); ++p1;
        }
        else if (list1[p1] > list2[p2]) {
            push_back(list2[p2]); ++p2;
        }
        else {
            push_back(list1[p1]); ++p1;
            push_back(list2[p2]); ++p2;
        }
    }
    while (p1 < list1.size()) {
        push_back(list1[p1]); ++p1;
    }
    while (p2 < list2.size()) {
        push_back(list2[p2]); ++p2;
    }
}

template<class T>
void ArrayList<T>::split(ArrayList<T>& list1, ArrayList<T>& list2) const {
    list1.clear();
    list2.clear();
    for (int i = 0; i < size(); ++i) {
        if (i & 1) {
            list2.push_back(data[i]);
        }
        else {
            list1.push_back(data[i]);
        }
    }
}