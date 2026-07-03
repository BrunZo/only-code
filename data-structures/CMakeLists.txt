cmake_minimum_required(VERSION 3.15)
project(data_structures)

set(CMAKE_CXX_STANDARD 20)

find_package(Python 3.8 COMPONENTS Interpreter Development.Module REQUIRED)
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(segtree segtree/bindings.cpp segtree/segtree.cpp)
target_include_directories(segtree PRIVATE segtree)

Python_add_library(csegtree MODULE WITH_SOABI csegtree/segtree.c)
target_include_directories(csegtree PRIVATE csegtree)

install(TARGETS segtree LIBRARY DESTINATION data_structures)
install(TARGETS csegtree LIBRARY DESTINATION data_structures)
