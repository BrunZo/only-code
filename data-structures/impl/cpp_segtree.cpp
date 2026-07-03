#include <chrono>
#include <iostream>
#include <cstring>
#include <cstdlib>

struct segtree {
    int size;
    int* tree;

    segtree(int size) : size(size), tree(new int[2 * size]) {
        memset(tree, 0, sizeof(int) * 2 * size);
    }

    void set(int pos, int val) {
        pos += size;
        tree[pos] = val;
        while (pos > 1) {
            tree[pos>>1] = tree[pos] + tree[pos^1];
            pos >>= 1;
        }
    }

    int query(int l, int r) {
        int res = 0;
        l += size;
        r += size;
        while (l < r) {
            if (l & 1) res += tree[l++];
            if (r & 1) res += tree[--r];
            l >>= 1;
            r >>= 1;
        }
        return res;
    }

    ~segtree() {
        delete[] tree;
    }
};

void stress_test(int n, int query_ops, int iter_ops) {
    // test 500000 queries on tree size 100000
    segtree st(n);
    for (int i = 0; i < n; i++) {
        st.set(i, random.randint(0, 100));
    }
    for (int i = 0; i < query_ops; i++) {
        int l = random.randint(0, n);
        int r = random.randint(l, n);
        st.query(l, r);
    }
    for (int i = 0; i < iter_ops; i++) {
        for (int j = 0; j < n; j++) {
            st.query(j, j + 1);
        }
}

int main() {
    stress_test(100000, 500000, 100);
    return 0;
}