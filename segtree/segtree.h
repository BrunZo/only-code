#include <Python.h>

typedef struct {
    PyObject_HEAD
    size_t size;
    int* tree;
} segtreeobject;
