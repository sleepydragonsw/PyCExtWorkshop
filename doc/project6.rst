Project 6: Releasing The GIL
============================

Introduction
------------

In this project you will extend the "hello you" function
to release the global interpreter lock during the printf call.


The GIL
-------

One of the highly controversial aspects of the Python programming language
is a concept called the "Global Interpreter Lock", or simply "The GIL".
As its name implies, only one Python thread at a time can hold the GIL
and therefore can proceed;
all other Python threads must wait in line for the GIL
before they can execute.
This has serious implications for multithreaded Python applications
that perform a lot of CPU computation
and causes them to perform badly compared to languages without such a lock
like Java and C++.

The GIL only needs to be held during times when Python objects
are being created, deleted, accessed, or manipulated.
In order to allow other threads to run,
Python C extension modules should release the GIL
whenever they perform an operation that does not require Python objects.
This is especially true for I/O operations, like ``printf``,
which actually take a "long time" to run
and does not require access to any Python objects.

To release the GIL use the
`Py_BEGIN_ALLOW_THREADS <http://docs.python.org/2/c-api/init.html#Py_BEGIN_ALLOW_THREADS>`_
macro.
To re-acquire the GIL use the
`Py_END_ALLOW_THREADS <http://docs.python.org/2/c-api/init.html#Py_END_ALLOW_THREADS>`_
macro.

As an example, here is how to use the macro with the ``printf()`` function:

.. code-block:: c

   Py_BEGIN_ALLOW_THREADS
   nCharsPrinted = printf("Hello %s\n", name);
   Py_END_ALLOW_THREADS

The complete, robust, and multithreading-friendly ``hello_you()`` function is now:

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
        Py_BEGIN_ALLOW_THREADS
        nCharsPrinted = printf("Hello %s\n", name);
        Py_END_ALLOW_THREADS
        retval = PyInt_FromLong(nCharsPrinted);
        return retval;
    }

Notice the introduction of
``Py_BEGIN_ALLOW_THREADS`` and ``Py_END_ALLOW_THREADS``
around the call to ``printf()``.


Compile and Try It
------------------

Compile your module and give it a try!
There should be no noticable difference from the last project
but you can feel good that you are doing your part
to minimize the negative effects of the GIL on multithreaded programs.

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

