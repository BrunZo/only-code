cmake_minimum_required(VERSION 3.15)
project(data_structures)

set(CMAKE_CXX_STANDARD 20)

find_package(Python 3.8 COMPONENTS Interpreter Development.Module REQUIRED)
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(segtree data_structures/segtree/bindings.cpp data_structures/segtree/segtree.cpp)
target_include_directories(segtree PRIVATE data_structures/segtree)

Python_add_library(csegtree MODULE WITH_SOABI data_structures/csegtree/segtree.c)
target_include_directories(csegtree PRIVATE data_structures/csegtree)

install(TARGETS segtree LIBRARY DESTINATION data_structures)
install(TARGETS csegtree LIBRARY DESTINATION data_structures)
