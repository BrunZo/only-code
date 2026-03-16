#include <Python.h>
#include <stddef.h>

typedef struct {
    PyObject_HEAD
    size_t size;
    int* tree;
} segtreeobject;

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
}

static PyMemberDef segtree_members[] = {
    {"size", Py_T_PYSSIZET, offsetof(segtreeobject, size), 0, "size"},
    {NULL},
};

static Py_ssize_t segtree_len(PyObject* op)
{
    segtreeobject* self = (segtreeobject*) op;
    return self->size;
};

static PyObject* segtree_set_one(PyObject *op, PyObject *args)
{
    segtreeobject* self = (segtreeobject*) op;
    int pos, val;
    if (!PyArg_ParseTuple(args, "ii", &pos, &val)) {
        return NULL;
    }
    pos += self->size;
    self->tree[pos] = val;
    while (pos > 1) {
        self->tree[pos>>1] = self->tree[pos] + self->tree[pos^1];
        pos >>= 1;
    } 

    Py_RETURN_NONE;
}

static PyObject* segtree_query(PyObject *op, PyObject *args)
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
    {"set_one", segtree_set_one, METH_VARARGS, "Set a value in a position"},
    {"query", segtree_query, METH_VARARGS, "Query a range [l, r)"},
    {NULL, NULL, 0, NULL},
};

static PySequenceMethods segtree_sequence_methods = {
    .sq_length = segtree_len,
};

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
    .tp_as_sequence = &segtree_sequence_methods,
    .tp_members = segtree_members,
    .tp_methods = segtree_methods,
};

static int
segtree_module_exec(PyObject *m)
{
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
