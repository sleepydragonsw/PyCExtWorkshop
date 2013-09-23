Project 5: How Many Characters?
===============================

Introduction
------------

In this project you will extend the "hello you" function
to return the number of characters printed to standard output.
This will introduce you to the way to return values to Python from C.


Returning the Value
-------------------

A lesser known fact about the *printf* C function
is that it actually returns an integer.
This integer indicates the number of characters written.
We are going to modify ``hello_you()`` to also return this number
instead of returning None.

The main piece of knowledge required is how to convert
a C int to a Python int object.
To do that, use the
`PyInt_FromLong <http://docs.python.org/2/c-api/int.html#PyInt_FromLong>`_
function:

.. code-block:: c

   PyObject* PyInt_FromLong(long ival)

As you can see, ``PyInt_FromLong()`` returns a ``PyObject *``
and conveniently ``hello_you()`` *also* returns a ``PyObject *``.
This means that we can simply return the ``PyObject *``
returned from ``PyInt_FromLong()`` from ``hello_you()``.

With this change, the ``hello_you()`` function becomes:


.. code-block:: c

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
        nCharsPrinted = printf("Hello %s\n", name);
        retval = PyInt_FromLong(nCharsPrinted);
        return retval;
    }

Note how the return value of ``printf`` is saved into
a ``long`` type variable named ``nCharsPrinted``.
Then, ``PyInt_FromLong`` is invoked with ``nCharsPrinted``
to create a ``PyObject *`` that represents the value of ``nCharsPrinted``.
Then, the ``PyObject *`` is returned.
Recall how previously the function used the ``Py_RETURN_NONE;`` macro
to return ``None`` from ``hello_you()``.


Compile and Try It
==================

Compile your module and give it a try!

.. code-block:: text

    c:\dev\cpyextworkshop>python_d setup.py build_ext --inplace --debug
    running build_ext
    building 'denver' extension
    c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN\cl.exe /c /nologo /Od /MDd /W3 /GS- /Z7 /D_DEBUG -Ic:\dev\py\Python-2.7.5\include -Ic:\dev\py\Python-2.7.5\PC /Tcc:\dev\cpyextworkshop\denver.c /Fobuild\temp.win32-2.7-pydebug\Debug\denver.obj denver.c
    c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN\link.exe /DLL /nologo /INCREMENTAL:no /DEBUG /pdb:None /LIBPATH:c:\dev\py\Python-2.7.5\libs /LIBPATH:c:\dev\py\Python-2.7.5\PCbuild /EXPORT:initdenver build\temp.win32-2.7-pydebug\Debug\denver.obj /OUT:c:\dev\cpyextworkshop\denver_d.pyd /MANIFEST /IMPLIB:build\temp.win32-2.7-pydebug\Debug\denver_d.lib /MANIFESTFILE:build\temp.win32-2.7-pydebug\Debug\denver_d.pyd.manifest
       Creating library build\temp.win32-2.7-pydebug\Debug\denver_d.lib and object build\temp.win32-2.7-pydebug\Debug\denver_d.exp
    [36809 refs]

    c:\dev\cpyextworkshop>python_d
    Python 2.7.5 (default, Jul  1 2013, 15:26:31) [MSC v.1600 32 bit (Intel)] on win32
    Type "help", "copyright", "credits" or "license" for more information.
    >>> import denver
    [43251 refs]
    >>> denver.hello_you("Mark")
    Hello Mark
    11
    [43253 refs]
    >>>

Notice how the call to ``hello_you()`` returned ``11``,
when previously it had no return value.
