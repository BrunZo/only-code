from setuptools import setup, Extension

# Define the extension module
module = Extension("segtree", sources=["segtree/segtree.c"])

setup(
    name="Segment Tree",
    version="0.1",
    description="Python C API package",
    ext_modules=[module],
)
