from setuptools import Extension, setup

# Define the extension module
module = Extension("segtree.segtree", sources=["segtree/segtree.c"])

setup(
    name="Segment Tree",
    version="0.1",
    description="Segment Tree implementation",
    ext_modules=[module],
)
