#include <Python.h>

static PyObject *
denver_hello_world(
    PyObject *module,
    PyObject *args
) {
    printf("Hello World\n");
    Py_RETURN_NONE;
}

static PyMethodDef denver_functions[] = {
    {"hello_world", denver_hello_world, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initdenver(void) {
    Py_InitModule("denver", denver_functions);
}
