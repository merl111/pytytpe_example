#define PY_SSIZE_T_CLEAN
#ifndef Py_TPFLAGS_CHECKTYPES
  #define Py_TPFLAGS_CHECKTYPES 0
#endif

#include <Python.h>
#include "longobject.h"
#include "structmember.h"
#include <stddef.h>

typedef struct {
    PyLongObject base;
    int state;
} MyObject;

static int
MyObject_init(MyObject *self, PyObject *args, PyObject *kwds)
{
    printf("init now \n");
    //self->state = 121212; // --> everything correct if commented, but state has the value of the third limb
    self->state = 121212; // --> third limb gets overwritten 
    printf("init done \n");

    return 0;
}

static PyObject *
MyObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    printf("new now\n");
    
    static char *kwlist[] = {"num", NULL};                                                                                                                                                                      
    PyObject *num = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist,                                                                                                                                                                    
                                     &num))                                                                          

    printf("PyLongObject type: %s\n", num->ob_type->tp_name);
    PyObject *tmp = _PyLong_Format(num, 10);
    PyObject *tmp2 = PyUnicode_AsASCIIString(tmp);


    PyObject *args2 = PyTuple_New(1);
    PyTuple_SetItem(args2, 0, tmp2);

    PyObject *o = (&PyLong_Type)->tp_new(type, args2, NULL);

    printf("NUM size: %ld\n", Py_SIZE(num));
    for (int i=0; i<Py_SIZE(num); i++) {
        printf("before init ob_digit: %u\n", ((PyLongObject *)o)->ob_digit[i]);
    }

    int ret = MyObject_init((MyObject *)o, args, kwds);

    for (int i=0; i<Py_SIZE(o); i++) {
        printf("after init ob_digit[%d]: %u\n", i, ((PyLongObject *)o)->ob_digit[i]);
    }

    printf("state: %d\n", ((MyObject *)o)->state);
    printf("new done %d \n", ret);

    return (PyObject *) o;
}

static PyTypeObject MyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "MyType.myobject",
    .tp_doc = "objects",
    .tp_basicsize = sizeof(MyObject),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES | Py_TPFLAGS_BASETYPE,
    .tp_new = (newfunc) MyObject_new,
};

static PyModuleDef mytype_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mytype",
    .m_doc = "Example module that creates an extension type.",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_mytype(void)
{
    PyObject *m;
    MyType.tp_base = &PyLong_Type;
    if (PyType_Ready(&MyType) < 0)
        return NULL;

    m = PyModule_Create(&mytype_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&MyType);
    if (PyModule_AddObject(m, "MyObject", (PyObject *) &MyType) < 0) {
        Py_DECREF(&MyType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
