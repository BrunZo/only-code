from setuptools import setup, Extension

# Define the extension module
module = Extension("segtree", sources=["segtree.c"])

setup(
    name="Segtree",
    version="1.0",
    description="Python C API package",
    ext_modules=[module],
)
