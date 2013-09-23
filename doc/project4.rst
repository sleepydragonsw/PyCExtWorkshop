Project 4: Hello, You
=====================

Introduction
------------

In this project you will extend the "hello world" function
to take a name as an argument and say hello to that person.
You will learn how to parse arguments passed from Python in C functions
and also how to convert then to native C types.


Define the Function
-------------------

Let's start by copying the ``hello_world()`` function
and calling it ``hello_you()``.

.. code-block:: c

    static PyObject *
    denver_hello_you(
        PyObject *module,
        PyObject *args
    ) {
        printf("Hello World\n");
        Py_RETURN_NONE;
    }

Parse arguments with PyArg_ParseTuple()
---------------------------------------

Each C function that is callable from Python
accepts 2 arguments:

  1. ``module`` -- a pointer to the module object
  2. ``args`` -- a Python tuple of the arguments given to the function

For the most part, the first argument can be ignored.
The second one, however, is immeasurably useful.
As mentioned above, the ``args`` argument is a Python tuple
that contains the arguments with which the function was invoked.
Although it is possible to take the values out of the tuple one-by-one,
validate them, convert them to equivalent C types, then use them,
this is such a common idiom that there is a helper function for just this purpose:
`PyArg_ParseTuple <http://docs.python.org/2/c-api/arg.html#PyArg_ParseTuple>`_

.. code-block:: c

    int PyArg_ParseTuple(PyObject *args, const char *format, ...)

``PyArg_ParseTuple()`` accepts the following arguments:

  1. ``args`` -- the arguments tuple with the arguments to parse
  2. ``format`` -- a printf-style format indicating the expected arguments
  3. the rest of the arguments are
     pointers to variables in which to store
     the parsed and converted arguments, according to the given format

For the first argument we will just pass on the ``args``
with which our C function was invoked.
The second argument, the ``format``, is both powerful and complicated.
It is fully documented at http://docs.python.org/2/c-api/arg.html.
But for our purposes, the method will just accept a single string argument
(the name of the person to say hello to).
Therefore, we will use the relatively simple format ``"s"``.
This format requires an additional ``const char **`` argument
into which a pointer to the parsed string argument will be stored.

So the argument parsing is done like this:

.. code-block:: c

   const char *name;
   PyArg_ParseTuple(args, "s", &name);

After having parsed the arguments, we can include the name in the printf message:

.. code-block:: c

   printf("Hello %s\n", name);

Piecing these code snippets together,
the ``hello_you()`` method in the C extension module becomes:

.. code-block:: c

   static PyObject *
   denver_hello_you(
       PyObject *module,
       PyObject *args
   ) {
       const char *name;
       PyArg_ParseTuple(args, "s", &name);
       printf("Hello %s\n", name);
       Py_RETURN_NONE;
   }


Handling Invalid Arguments
--------------------------

There is one thing left to handle in order to make ``hello_you()`` fully robust.
As it is currently implemented
it assumes that it will only ever be invoked with one argument,
and that the argument will be a string.
But what will happen if it is invoked by accident (or on purpose)
with zero arguments, or 2 arguments, or one integer argument?
As it currently stands, it will probably crash the Python interpreter.
But this is very easy to fix.

``PyArg_ParseTuple()`` returns an integer:
0 if parsing fails and non-zero if parsing succeeds.
Moreover, if it returns 0 then it also raises an appropriate Python exception.
So all you have to do is immediately return NULL if ``PyArg_ParseTuple()`` fails.

So the fully robust version of the method becomes:

.. code-block:: c

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

Add the Function to the Function List
-------------------------------------

Do you remember adding ``hello_world()`` to the *PyMethodDef* list
in an earlier project?
We have to do that same thing for ``hello_you()``.

One important difference between ``hello_world()`` and ``hello_you()``
is that ``hello_world()`` did not accept any arguments,
whereas ``hello_you()`` accepts exactly one argument.
You may recall using the flag ``METH_NOARGS`` with ``hello_world()``
to indicate that it accepts no arguments.
For the ``hello_you()`` function you must instead use the flag ``METH_VARARGS``
to indicate that it accepts arguments.

So the entry in the *PyMethodDef* function list for ``hello_you()`` will be:

.. code-block:: c

   {"hello_you", denver_hello_you, METH_VARARGS, NULL}

Add this line into the *PyMethodDef* list, to become:

.. code-block:: c

    static PyMethodDef denver_functions[] = {
        {"hello_world", denver_hello_world, METH_NOARGS, NULL},
        {"hello_you", denver_hello_you, METH_VARARGS, NULL},
        {NULL, NULL, 0, NULL}
    };


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
    [43251 refs]
    >>>

It also correctly raises exceptions when invalid arguments are given:

.. code-block:: text

    >>> denver.hello_you()
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    TypeError: function takes exactly 1 argument (0 given)
    [43277 refs]
    >>> denver.hello_you("a", "b")
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    TypeError: function takes exactly 1 argument (2 given)
    [43279 refs]
    >>> denver.hello_you(123)
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    TypeError: must be string, not int
    [43278 refs]
    >>>

Your ``hello_you()`` function is now complete for this project.
