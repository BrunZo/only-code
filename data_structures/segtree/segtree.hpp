#pragma once

#include <cstddef>
#include <functional>
#include <vector>


template<typename T>
class segtree
{
    size_t size;
    T id;
    std::function<T(T, T)> op;
    std::vector<T> data;

public:
    segtree(size_t size, T id, std::function<T(T, T)> op);

    void build(std::vector<T> leaves);

    void set(int pos, T val);
    T query(int l, int r);

    size_t len();
    T getitem(size_t i);
    void setitem(size_t i, T val);

    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
};
