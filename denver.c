#include <Python.h>

static PyObject *
denver_hello_world(
    PyObject *module,
    PyObject *args
) {
    printf("Hello World\n");
    Py_RETURN_NONE;
}

static PyObject *
denver_hello_you(
    PyObject *module,
    PyObject *args
) {
    const char *name;
    if (! PyArg_ParseTuple(args, "s", &name)) {
        return NULL;
    }
    printf("Hello %s\n", name);
    Py_RETURN_NONE;
}

static PyMethodDef denver_functions[] = {
    {"hello_world", denver_hello_world, METH_NOARGS, NULL},
    {"hello_you", denver_hello_you, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initdenver(void) {
    Py_InitModule("denver", denver_functions);
}
