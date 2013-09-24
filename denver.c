#include <Python.h>
#include <png.h>
#include <setjmp.h>

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
    long nCharsPrinted;
    PyObject *retval;
    if (! PyArg_ParseTuple(args, "s", &name)) {
        return NULL;
    }
    Py_BEGIN_ALLOW_THREADS
    nCharsPrinted = printf("Hello %s\n", name);
    Py_END_ALLOW_THREADS
    retval = PyInt_FromLong(nCharsPrinted);
    return retval;
}

static void
denver_png_read_fn(
    png_structp png_ptr,
    png_bytep data,
    png_size_t length
) {
    FILE *f;
    png_size_t check;
    f = (FILE*) png_get_io_ptr(png_ptr);
    check = fread(data, 1, length, f);
    if (check < length) {
        if (ferror(f)) {
            PyErr_SetFromErrno(PyExc_IOError);
        } else {
            PyErr_Format(PyExc_IOError,
                "premature end-of-file reached; "
                "wanted %i bytes but got %i ",
                length, check);
        }
        png_longjmp(png_ptr, 1);
    }
}

static void
denver_png_error_fn(
    png_structp png_ptr,
    png_const_charp message
) {
    if (! PyErr_Occurred()) {
        PyErr_SetString(PyExc_ValueError, message);
    }
    png_longjmp(png_ptr, 1);
}

static PyObject *
denver_png_dimensions(
    PyObject *module,
    PyObject *args
) {
    FILE *f;
    char *path;
    size_t n;
    char buf[8];
    int isPng;
    png_structp png_ptr = NULL;
    png_inforp info_ptr = NULL;
    png_uint_32 image_width, image_height;
    PyObject *retval, *widthObj, *heightObj;

    if (! PyArg_ParseTuple(args, "s", &path)) {
        return NULL;
    }

    f = fopen(path, "rb");
    if (!f) {
        PyErr_SetFromErrno(PyExc_IOError);
        return NULL;
    }

    n = fread(buf, 1, 8, f);
    if (n < 8 && ferror(f)) {
        PyErr_SetFromErrno(PyExc_IOError);
        fclose(f);
        return NULL;
    }

    isPng = png_check_sig(buf, n);
    if (! isPng) {
        fclose(f);
        PyErr_Format(PyExc_ValueError,
            "first %i bytes of file do not consitute a valid PNG signature",
            n);
        return NULL;
    }

    png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (! png_ptr) {
        fclose(f);
        PyErr_NoMemory();
        return NULL;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (! info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(f);
        PyErr_NoMemory();
        return NULL;
    }

    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(f);
        return NULL;
    }

    png_set_read_fn(png_ptr, f, denver_png_read_fn);
    png_set_error_fn(png_ptr, NULL, denver_png_error_fn, NULL);
    png_set_sig_bytes(png_ptr, n);

    png_read_info(png_ptr, info_ptr);
    image_width = png_get_image_width(png_ptr, info_ptr);
    image_height = png_get_image_height(png_ptr, info_ptr);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(f);

    widthObj = PyInt_FromLong(image_width);
    if (widthObj == NULL) {
        PyErr_NoMemory();
        return NULL;
    }

    heightObj = PyInt_FromLong(image_height);
    if (heightObj == NULL) {
        Py_DECREF(widthObj);
        PyErr_NoMemory();
        return NULL;
    }

    retval = PyTuple_New(2);
    if (retval == NULL) {
        Py_DECREF(widthObj);
        Py_DECREF(heightObj);
        PyErr_NoMemory();
        return NULL;
    }

    PyTuple_SET_ITEM(retval, 0, widthObj);
    PyTuple_SET_ITEM(retval, 1, heightObj);
    return retval;
}

static PyMethodDef denver_functions[] = {
    {"hello_world", denver_hello_world, METH_NOARGS, NULL},
    {"hello_you", denver_hello_you, METH_VARARGS, NULL},
    {"png_dimensions", denver_png_dimensions, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initdenver(void) {
    Py_InitModule("denver", denver_functions);
}
