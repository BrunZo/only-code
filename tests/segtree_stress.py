import argparse
import operator
import random
import time

from data_structures.csegtree import segtree as CSegTree
from data_structures.segtree import segtree as CppSegTree


def parse_args():
    ap = argparse.ArgumentParser(description="Benchmark csegtree (C) vs segtree (C++) implementations")
    ap.add_argument("--n", type=int, default=100_000)
    ap.add_argument("--query_ops", type=int, default=500_000)
    ap.add_argument("--iter_ops", type=int, default=1_000)
    return ap.parse_args()


def main():
    args = parse_args()
    stress_test(args.n, args.query_ops, args.iter_ops)


def stress_test(n: int, query_ops: int, iter_ops: int):
    leaves = [random.randint(0, 100) for _ in range(n)]
    bench("csegtree", CSegTree, n, query_ops, iter_ops, leaves)
    bench("segtree", CppSegTree, n, query_ops, iter_ops, leaves)


def bench(name, ctor, n, query_ops, iter_ops, leaves):
    start = time.perf_counter()
    st = ctor(n, 0, operator.add)
    st.build(leaves)
    build_time = time.perf_counter() - start

    random.seed(1234)
    start = time.perf_counter()
    for _ in range(query_ops):
        l = random.randint(0, n)
        r = random.randint(l, n)
        st.query(l, r)
    query_time = time.perf_counter() - start

    start = time.perf_counter()
    for _ in range(iter_ops):
        for _ in st:
            pass
    iter_time = time.perf_counter() - start

    print(
        f"{name:>10}  build={build_time:8.4f}s  "
        f"query={query_time:8.4f}s ({query_ops / query_time:,.0f} ops/s)  "
        f"iter={iter_time:8.4f}s ({iter_ops * n / iter_time:,.0f} items/s)"
    )


if __name__ == "__main__":
    main()
