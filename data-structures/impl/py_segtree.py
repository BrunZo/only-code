
import argparse
import operator
import cProfile as cprof
import random


class segtree:

    def __init__(self, size, identity, op):
        self.size = size
        self.tree = [identity] * (2 * size)
        self.identity = identity
        self.op = op

    def __len__(self):
        return self.size

    def __iter__(self):
        return iter(self.tree[self.size : 2 * self.size])
    
    def __getitem__(self, pos):
        return self.tree[self.size + pos]

    def __setitem__(self, pos, val):
        pos += self.size
        self.tree[pos] = val
        while pos > 1:
            self.tree[pos >> 1] = self.op(self.tree[pos], self.tree[pos ^ 1])
            pos >>= 1

    def query(self, l, r):
        res = self.identity
        l += self.size
        r += self.size
        while l < r:
            if l & 1:
                res = self.op(res, self.tree[l])
                l += 1
            if r & 1:
                r -= 1
                res = self.op(res, self.tree[r])
            l >>= 1
            r >>= 1
        return res


def stress_test(n: int, query_ops: int, iter_ops: int):

    st = segtree(n, 0, operator.add)
    for i in range(n):
        st[i] = random.randint(0, 100)

    for i in range(query_ops):
        l = random.randint(0, n)
        r = random.randint(l, n)
        st.query(l, r)

    for _ in range(iter_ops):
        for _ in st:
            pass


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--n", type=int, default=100_000)
    parser.add_argument("--query_ops", type=int, default=500_000)
    parser.add_argument("--iter_ops", type=int, default=1_000)
    args = parser.parse_args()
    cprof.run("stress_test(args.n, args.query_ops, args.iter_ops)")
