import argparse
import pandas as pd
import random
import cProfile as cprof

from segtree import segtree

def concat_df_test(n: int, query_ops: int):

    st = segtree(n, pd.DataFrame(), lambda x, y: pd.concat([x, y]))
    for i in range(n):
        st[i] = pd.DataFrame({
            "a": [random.randint(0, 100)],
            "b": [random.randint(0, 100)],
            "c": [random.randint(0, 100)],
        })
    
    for _ in range(query_ops): 
        l = random.randint(0, n)
        r = random.randint(l, n)
        st.query(l, r)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--n", type=int, default=1_000)
    parser.add_argument("--query_ops", type=int, default=5_000)
    args = parser.parse_args()
    cprof.run("concat_df_test(args.n, args.query_ops)")
