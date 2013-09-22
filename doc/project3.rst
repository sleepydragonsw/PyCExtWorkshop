Project 3: Hello World
======================

Introduction
------------

In this project you will implement the classical introductory algorithm: hello world.
This project will introduce the basics of writing Python C extension functions
by simply printing "Hello World" to standard output when invoked.

Define the Function
-------------------

When implementing a Python function in C
it must conform to the definition of *PyCFunction*
which is specified in *Python.h* :

.. code-block:: c

    typedef PyObject *(*PyCFunction)(PyObject *, PyObject *);

If this syntax is unfamiliar to you,
it just means that you must write a function
that accepts two pointers to PyObject structs
and returns a pointer to a PyObject struct.
The first argument is a pointer to the "context" of the function call,
which in this case is a pointer to the module to which the function belongs.
For an method of a class it would be a pointer to the object on which the method was invoked.
The second argument is a tuple object that contains the function's arguments.
The returned object will be the return value of the function,
or NULL if a Python exception was raised.
You must always remember that in Python *everything is an object*.
Objects are of course objects,
but so are ints, floats, strings, lists, dicts, True, False, and even None.

Below is one way to write the "hello world" function
that is capable of being called from Python.
This code should immediately follow the *#include <Python.h>* line in *denver.c*:

.. code-block:: c

    static PyObject *
    denver_hello_world(
        PyObject *module,
        PyObject *args
    ) {
        printf("Hello World\n");
        Py_RETURN_NONE;
    }

Note that normally you would need to *#include <stdio.h>*
in order to use the *printf* function
but as it turns out this is included in *Python.h* and therefore is not strictly necessary.

As a sanity check, try and run *setup.py* to compile your code.

.. code-block:: text

    python_d setup.py build_ext --inplace --debug

At this time it will compile, but there is still another piece of the puzzle to complete
before your shiny new method is available from Python.

Add the Function to the Function List
-------------------------------------

Do you remember the *PyMethodDef* list that ended with ``{NULL, NULL, 0, NULL}``
that you added to your C file an earlier project?
This is the list to which your new C function needs to be added
in order to make it callable from Python.

Here is the new code to use in *denver.c* to expose your function to Python:

.. code-block:: c

    static PyMethodDef denver_functions[] = {
        {"hello_world", denver_hello_world, METH_NOARGS, NULL},
        {NULL, NULL, 0, NULL}
    };

A *PyMethodDef* struct has 4 members:

    *ml_name* (const char \*)
        the name by which this function will be callable from Python.
    *ml_meth* (PyCFunction)
        the C function that is to be invoked when called from Python.
    *ml_flags* (int)
        flags that affect how the C function is invoked;
        for example, ``METH_NOARGS`` indicates that the function accepts no arguments
        and ``METH_VARARGS`` indicates that the function accepts positional arguments
        but not keyword arguments.
    *ml_doc* (const char \*)
        the docstring for the function;
        this string will be available from Python
        via the ``__doc__`` attribute of the function;
        may be ``NULL`` to not add a docstring to the function.

With this change, the ``denver_hello_world`` C function
can be called from Python.
Compile your module and give it a try!

.. code-block:: text

    c:\dev\cpyextworkshop>python_d setup.py build_ext --inplace --debug
    running build_ext
    building 'denver' extension
    c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN\cl.exe /c /nologo /Od /MDd /W3 /GS- /Z7 /D_DEBUG -Ic:\dev\Python-2.7.5-VS2010\Python-2.7.5\include -Ic:\dev\Python-2.7.5-VS2010\Python-2.7.5\PC /Tcc:\dev\cpyextworkshop\denver.c /Fobuild\temp.win32-2.7-pydebug\Debug\denver.obj denver.c
    c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN\link.exe /DLL /nologo /INCREMENTAL:no /DEBUG /pdb:None /LIBPATH:c:\dev\Python-2.7.5-VS2010\Python-2.7.5\libs /LIBPATH:c:\dev\Python-2.7.5-VS2010\Python-2.7.5\PCbuild /EXPORT:initdenver build\temp.win32-2.7-pydebug\Debug\denver.obj /OUT:c:\dev\cpyextworkshop\denver_d.pyd /IMPLIB:build\temp.win32-2.7-pydebug\Debug\denver_d.lib /MANIFESTFILE:build\temp.win32-2.7-pydebug\Debug\denver_d.pyd.manifest /MANIFEST
       Creating library build\temp.win32-2.7-pydebug\Debug\denver_d.lib and object build\temp.win32-2.7-pydebug\Debug\denver_d.exp
    [36800 refs]

    c:\dev\cpyextworkshop>python_d
    Python 2.7.5 (default, Aug 11 2013, 10:23:02) [MSC v.1600 32 bit (Intel)] on win32
    Type "help", "copyright", "credits" or "license" for more information.
    >>> import denver
    [43202 refs]
    >>> denver.hello_world()
    Hello World
    [43202 refs]
    >>>

Notice how "Hello World" was printed when the ``hello_world()`` was invoked.
