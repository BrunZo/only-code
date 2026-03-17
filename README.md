# segtree

An implementation of a segment tree in Python using the C API. C speed and Python versatility.

## Usage

Constructor takes three arguments, the size of the segment tree, the identity element and the operation to be performed.

```python
import operator
from segtree import segtree

# Sum segment tree
st = segtree(100, 0, operator.add)

# Max segment tree
st = segtree(100, -inf, operator.max)

# List concatenation segment tree
st = segtree(100, [], lambda x, y: x + y)
```

Queries have a dedicated method:

```python
st.query(l, r)   # Query range [l, r)
```

Otherwise, this implementation of the segment tree behaves pretty much like a list (thanks to the Python data model)

```python
# Set/get values using indices
st[3] = value
print(st[9])

# You can also iterate the segment tree like a list
for value in st:
    print(value)
```

## Performance (`stress.py`)

Trees are of size 100,000. Query operations = 500,000. Iteration operations = 100.

| implementation | query time |
|----------------|-------|
| `py_segtree.py` | 2.886 s |
| `segtree.c` | 0.467 s |

Speedup is 6.18x.
