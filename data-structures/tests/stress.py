import argparse
import operator
import random

import cProfile as cprof

from segtree import segtree

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
