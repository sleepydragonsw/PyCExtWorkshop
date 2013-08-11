#include <Python.h>

static PyMethodDef denver_functions[] = {
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initdenver(void) {
    Py_InitModule("denver", denver_functions);
}
