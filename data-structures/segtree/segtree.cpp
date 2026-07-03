#include <nanobind/nanobind.h>
#include "segtree.hpp"

template<typename T>
segtree<T>::segtree(size_t size, T id, std::function<T(T, T)> op)
    : size(size), id(id), op(op) {
    data = std::vector<T>(2 * size, id);
}

template<typename T>
void segtree<T>::build(std::vector<T> leaves) {
    for (size_t i = 0; i < size; i++)
        data[size + i] = leaves[i];
    for (size_t i = size - 1; i > 0; i--)
        data[i] = op(data[2 * i], data[2 * i + 1]);
}

template<typename T>
void segtree<T>::set(int pos, T val) {
    pos += size;
    data[pos] = val;
    while (pos > 1) {
        pos >>= 1;
        data[pos] = op(data[2 * pos], data[2 * pos + 1]);
    }
}

template<typename T>
T segtree<T>::query(int l, int r) {
    T resl = id, resr = id;
    l += size;
    r += size;
    while (l < r) {
        if (l & 1) resl = op(resl, data[l++]);
        if (r & 1) resr = op(data[--r], resr);
        l >>= 1;
        r >>= 1;
    }
    return op(resl, resr);
}

template<typename T>
size_t segtree<T>::len() {
    return size;
}

template<typename T>
T segtree<T>::getitem(size_t i) {
    return data[i + size];
}

template<typename T>
void segtree<T>::setitem(size_t i, T val) {
    set(i, val);
}

template<typename T>
typename std::vector<T>::iterator segtree<T>::begin() {
    return data.begin() + size;
}

template<typename T>
typename std::vector<T>::iterator segtree<T>::end() {
    return data.end();
}

template class segtree<nanobind::object>;
