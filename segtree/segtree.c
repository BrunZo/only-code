#include <Python.h>
#include <stddef.h>
#include "segtree_iterator.h"

/* Object methods */

static PyObject*
segtree_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    segtreeobject* self = (segtreeobject*) type->tp_alloc(type, 0);    
    if (self != NULL) {
        self->size = 0;
    }
    return (PyObject*) self;
}

static int segtree_init(PyObject *op, PyObject *args, PyObject *kwds)
{
    segtreeobject *self = (segtreeobject*) op;
    PyObject *identity = NULL;
    PyObject *op_func = NULL;
    if (!PyArg_ParseTuple(args, "nOO", &self->size, &identity, &op_func)) {
        return -1;
    }
    if (!PyCallable_Check(op_func)) {
        PyErr_SetString(PyExc_TypeError, "op must be callable");
        return -1;
    }
    self->identity = identity;
    Py_INCREF(identity);
    self->op = op_func;
    Py_INCREF(op_func);

    self->tree = (PyObject**) PyMem_Calloc(2 * self->size, sizeof(PyObject*));
    if (self->tree == NULL) {
        PyErr_NoMemory();
        return -1;
    }
    for (Py_ssize_t i = 0; i < 2 * self->size; i++) {
        self->tree[i] = identity;
        Py_INCREF(identity);
    }
    return 0;
}

static void segtree_dealloc(PyObject *op)
{
    segtreeobject *self = (segtreeobject*) op;
    for (Py_ssize_t i = 0; i < 2 * self->size; i++) {
        Py_XDECREF(self->tree[i]);
    }
    PyMem_Free(self->tree);
    Py_XDECREF(self->identity);
    Py_XDECREF(self->op);
    Py_TYPE(self)->tp_free(self);
}

/* Iterator */

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
    PyObject *val = self->tree[self->size + index];
    Py_INCREF(val);
    return val;
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
    Py_XDECREF(self->tree[index]);
    self->tree[index] = value;
    Py_INCREF(value);
     
    while (index > 1) {
        PyObject *left = self->tree[index];
        PyObject *right = self->tree[index ^ 1];
        PyObject *combined = PyObject_CallFunction(self->op, "OO", left, right);
        if (combined == NULL) return -1;
        Py_XDECREF(self->tree[index >> 1]);
        self->tree[index >> 1] = combined;
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
    segtreeobject *self = (segtreeobject*) op;
    Py_ssize_t lf, rg;
    if (!PyArg_ParseTuple(args, "nn", &lf, &rg)) {
        return NULL;
    }
    lf += self->size;
    rg += self->size;
    PyObject *res = self->identity;
    Py_INCREF(res);

    while (lf < rg) {
        if (lf & 1) {
            PyObject *new_res = PyObject_CallFunction(self->op, "OO", res, self->tree[lf]);
            if (new_res == NULL) { Py_DECREF(res); return NULL; }
            Py_DECREF(res);
            res = new_res;
            lf++;
        }
        if (rg & 1) {
            rg--;
            PyObject *new_res = PyObject_CallFunction(self->op, "OO", res, self->tree[rg]);
            if (new_res == NULL) { Py_DECREF(res); return NULL; }
            Py_DECREF(res);
            res = new_res;
        }
        lf >>= 1;
        rg >>= 1;
    }
    return res;
};

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
