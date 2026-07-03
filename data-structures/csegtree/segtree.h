#include <Python.h>

typedef struct {
    PyObject_HEAD
    Py_ssize_t size;
    PyObject** tree;
    PyObject* identity;
    PyObject* op;
} segtreeobject;

static PyObject* segtree_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int segtree_init(PyObject *op, PyObject *args, PyObject *kwds);
static void segtree_dealloc(PyObject *op);
static PyObject* segtree_iter(PyObject* op);

static Py_ssize_t segtree_len(PyObject* op);
static PyObject* segtree_getitem(PyObject *op, Py_ssize_t index);
static int segtree_setitem(PyObject *op, Py_ssize_t index, PyObject *value);

static PyObject* segtree_query(PyObject *op, PyObject *args);
static PyObject* segtree_build(PyObject *op, PyObject *args);
