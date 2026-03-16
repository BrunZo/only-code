#include <Python.h>

typedef struct {
    PyObject_HEAD
    size_t size;
    int* tree;
} segtreeobject;

static PyObject* segtree_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int segtree_init(PyObject *op, PyObject *args, PyObject *kwds);
static void segtree_dealloc(PyObject *op);
static Py_ssize_t segtree_len(PyObject* op);
static PyObject* segtree_iter(PyObject* op);
static PyObject* segtree_set_one(PyObject *op, PyObject *args);
static PyObject* segtree_query(PyObject *op, PyObject *args);
