#include <Python.h>
#include <stddef.h>
#include "segtree_iterator.h"

/* Object methods */

static void
segtree_dealloc(PyObject *op)
{
    segtreeobject *self = (segtreeobject*) op;
    free(self->tree);
    Py_TYPE(self)->tp_free(self);
}

static PyObject*
segtree_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    segtreeobject* self = (segtreeobject*) type->tp_alloc(type, 0);    
    if (self != NULL) {
        self->size = 0;
    }
    return (PyObject*) self;
}

static int
segtree_init(PyObject *op, PyObject *args, PyObject *kwds)
{
    segtreeobject *self = (segtreeobject*) op;
    if (!PyArg_ParseTuple(args, "i", &self->size)) {
        return -1;
    }
    self->tree = (int*) malloc(sizeof(int) * 2 * self->size);
    if (self->tree == NULL) {
        PyErr_NoMemory();
        return -1;
    }
    memset(self->tree, 0, sizeof(int) * 2 * self->size);
    return 0;
};

static PyObject*
segtree_iter(PyObject* op)
{
    segtreeobject* self = (segtreeobject*) op;
    segtreeiterobject *it = PyObject_New(segtreeiterobject, &segtreeiter_type);
    if (it == NULL) {
        return NULL;
    }
    it->segtree = self;
    Py_INCREF(self);
    it->index = 0;
    return (PyObject *) it;
};

/* Sequence methods */

static Py_ssize_t
segtree_len(PyObject* op)
{
    segtreeobject* self = (segtreeobject*) op;
    return self->size;
};

static PyObject*
segtree_getitem(PyObject *op, Py_ssize_t index)
{
    segtreeobject *self = (segtreeobject*) op;
    if (index < 0 || index >= self->size) {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        return NULL;
    }
    return PyLong_FromLong(self->tree[self->size + index]);
};

static int
segtree_setitem(PyObject *op, Py_ssize_t index, PyObject *value)
{
    segtreeobject *self = (segtreeobject*) op;
    if (index < 0 || index >= self->size) {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        return -1;
    }
    index += self->size;    
    self->tree[index] = PyLong_AsLong(value);
    while (index > 1) {
        self->tree[index>>1] = self->tree[index] + self->tree[index^1];
        index >>= 1;
    }
    return 0;
};

static PySequenceMethods segtree_sequence_methods = {
    .sq_length = segtree_len,
    .sq_item = segtree_getitem,
    .sq_ass_item = segtree_setitem,
};

/* Methods */

static PyObject*
segtree_query(PyObject *op, PyObject *args)
{
    segtreeobject* self = (segtreeobject*) op;
    int lf, rg;
    if (!PyArg_ParseTuple(args, "ii", &lf, &rg)) {
        return NULL;
    }
    lf += self->size;
    rg += self->size;
    int pf = 0, sf = 0;
    while (lf < rg) {
        if (lf & 1) pf = pf + self->tree[lf++];
        if (rg & 1) sf = self->tree[--rg] + sf;
        lf >>= 1; 
        rg >>= 1;
    }
    PyObject* response = PyLong_FromLong(pf + sf);
    return response;
}

static PyMethodDef segtree_methods[] = {
    {"query", segtree_query, METH_VARARGS, "Query a range [l, r)"},
    {NULL, NULL, 0, NULL},
};

/* Type object */

static PyTypeObject segtreetype = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "segtree.segtree",
    .tp_doc = PyDoc_STR("segtree"),
    .tp_basicsize = sizeof(segtreeobject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = segtree_new,
    .tp_init = segtree_init,
    .tp_dealloc = segtree_dealloc,
    .tp_iter = segtree_iter,
    .tp_as_sequence = &segtree_sequence_methods,
    .tp_methods = segtree_methods,
};

static int
segtree_module_exec(PyObject *m)
{
    if (PyType_Ready(&segtreeiter_type) < 0) {
        return -1;
    }

    if (PyType_Ready(&segtreetype) < 0) {
        return -1;
    }

    if (PyModule_AddObjectRef(m, "segtree", (PyObject *) &segtreetype) < 0) {
        return -1;
    }

    return 0;
};

static PyModuleDef_Slot segtree_module_slots[] = {
    {Py_mod_exec, segtree_module_exec},
    {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
    {0, NULL},
};

static struct PyModuleDef segtree_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "segtree",     // Module name
    .m_doc = "A simple segtree module", 
    .m_size = 0, 
    .m_slots = segtree_module_slots,
};

PyMODINIT_FUNC
PyInit_segtree(void) {
    return PyModuleDef_Init(&segtree_module);
}
