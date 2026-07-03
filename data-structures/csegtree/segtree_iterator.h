#include <Python.h>
#include "segtree.h"

typedef struct {
    PyObject_HEAD
    segtreeobject *segtree;
    Py_ssize_t index;
} segtreeiterobject;

static void
segtreeiter_dealloc(PyObject *op)
{
    segtreeiterobject *it = (segtreeiterobject *) op;
    Py_XDECREF(it->segtree);
    Py_TYPE(op)->tp_free(op);
}

static PyObject*
segtreeiter_iternext(PyObject *op)
{
    segtreeiterobject *it = (segtreeiterobject *) op;
    if (it->index >= (Py_ssize_t)it->segtree->size) {
        return NULL;
    }
    PyObject* val = it->segtree->tree[it->segtree->size + it->index];
    Py_INCREF(val);
    it->index++;
    return val;
}

static PyObject*
segtreeiter_iter(PyObject *op)
{
    Py_INCREF(op);
    return op;
}

static PyTypeObject segtreeiter_type = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "csegtree.segtree_iterator",
    .tp_basicsize = sizeof(segtreeiterobject),
    .tp_dealloc = segtreeiter_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_iter = segtreeiter_iter,
    .tp_iternext = segtreeiter_iternext,
};
