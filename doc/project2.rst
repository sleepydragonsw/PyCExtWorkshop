Project 2: Import Your Extension Module
=======================================

Introduction
------------

To write a C function that can be called from Python scripts
you must create what is called an *extension module*.
That is, you have to create a module that is entirely written in C
and import that module in your Python source code.
So as the "coding" part of this workshop
we will create an extension module with
zero functions, zero classes, zero attributes... an empty module.
If your extension module can be imported, you have completed this project.

Create denver.c
---------------

Let's create the actual C source code for your new extension module.
For the name of your extension module use your first name.
My name is Denver Coneybeare, so my extension module name will be *denver*
and will be contained in a file named *denver.c*.
Make sure to substitute each occurrence of "denver"
in the steps that follow with the name of your module.

Include Python.h
................

As the very first line of the C source file
include the standard Python header file, *Python.h*:

.. code-block:: c

    #include <Python.h>

It is strongly recommended for this to be the *very first* line of code in your file.
As documented
`in the Python standard documentation <http://docs.python.org/2/extending/extending.html#a-simple-example>`_,
the reason for this is that Python.h

    *may define some pre-processor definitions
    which affect the standard headers on some systems,
    you must include Python.h before any standard headers are included.*

Create Module's Function List
.............................

Unlike writing functions in Python,
when writing a C extension module
the functions that will be included in the module
need to be defined ahead of time.
Since our initial module will contain zero functions
we simply need to create an "emtpy list" of functions.
To do this, add the following definition to your C source code:

.. code-block:: c

    static PyMethodDef denver_functions[] = {
        {NULL, NULL, 0, NULL}
    };

`PyMethodDef <http://docs.python.org/2/c-api/structures.html?highlight=pymethoddef#PyMethodDef>`_
is a typedef defined in *Python.h*
as a structure that specifies the properties of a C function
that can be called from Python.
Namely, it specifies the name, C function, flags, and docstring.
We will visit this structure later on
when we actually add functions to our module.
For now, we create a list of PyMethodDef objects
that contains only the end-of-list sentinel, ``{NULL, NULL, 0, NULL}``.

Create the Module Object
........................

When a Python C extension module is loaded by the Python interpreter
it calls a specially-named function
that is responsible for creating the module object
and populating it with values, functions, and classes.
The name of this function is ``initdenver`` for a module named "denver"
and must have the following signature:

.. code-block:: c

    PyMODINIT_FUNC initdenver(void);

Implement this function as follows,
which creates the module object for your extension module:

.. code-block:: c

    PyMODINIT_FUNC
    initdenver(void) {
        Py_InitModule("denver", denver_functions);
    }

Create setup.py
---------------

The last step of creating a Python extension module
is to write a brief Python script that uses the built-in *distutils* module
to issue the necessary commands to compile the C code.
By convention, this file is named *setup.py*.

.. code-block:: python

    # -*- coding: utf-8 -*-

    import distutils.core

    ext_module = distutils.core.Extension(
        "denver",
        sources=["denver.c"],
    )

    distutils.core.setup(
        ext_modules=[ext_module],
    )

Of course, replace *denver* with the name of *your* extension module.

Compile Your Extension Module
-----------------------------

Now that you've finished writing the necessary C code
it's time to compile your module and see if it actually is importable!

It is strongly encouraged to use your self-compiled version of Python.
To do so, add the build directory to your PATH environment variable.
In Windows, instead add the *PCBuild* directory to your PATH.
On my computer, I ran the following:

.. code-block:: text

    set PATH=c:\dev\Python-2.7.5-VS2010\Python-2.7.5\PCbuild;%PATH%

Then, use *python_d.exe* (on Windows) or *python.exe* (on Mac and Linux)
to use your *setup.py* script to build the C extension module:

.. code-block:: text

    python_d setup.py build_ext --debug --inplace

On my computer, it produced this output:

.. code-block:: text

    c:\dev\cpyextworkshop>python_d setup.py build_ext --debug --inplace
    running build_ext
    building 'denver' extension
    c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN\cl.exe /c /nologo /Od /MDd /W3 /GS- /Z7 /D_DEBUG -Ic:\dev\Python-2.7.5-VS2010\Python-2.7.5\include -Ic:\dev\Python-2.7.5-VS2010\Python-2.7.5\PC /Tcc:\dev\cpyextworkshop\denver.c /Fobuild\temp.win32-2.7-pydebug\Debug\denver.obj
    denver.c
    c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN\link.exe /DLL /nologo /INCREMENTAL:no /DEBUG /pdb:None /LIBPATH:c:\dev\Python-2.7.5-VS2010\Python-2.7.5\libs /LIBPATH:c:\dev\Python-2.7.5-VS2010\Python-2.7.5\PCbuild /EXPORT:initdenver build\temp.win32-2.7-pydebug\Debug\denver.obj /OUT:c:\dev\cpyextworkshop\denver_d.pyd /IMPLIB:build\temp.win32-2.7-pydebug\Debug\denver_d.lib /MANIFESTFILE:build\temp.win32-2.7-pydebug\Debug\denver_d.pyd.manifest /MANIFEST
       Creating library build\temp.win32-2.7-pydebug\Debug\denver_d.lib and object build\temp.win32-2.7-pydebug\Debug\denver_d.exp

Finally, launch a Python interpreter and test out your newly-compiled module.
Remember to use *python_d.exe* on Windows, not *python.exe*.

.. code-block:: text

    c:\dev\cpyextworkshop>python_d
    Python 2.7.5 (default, Aug 11 2013, 10:23:02) [MSC v.1600 32 bit (Intel)] on win32
    Type "help", "copyright", "credits" or "license" for more information.
    >>> import denver
    [43195 refs]
    >>> dir(denver)
    ['__doc__', '__file__', '__name__', '__package__']
    [43206 refs]
    >>>

If you were able to import your module
then you have successfully completed this project.
Congratulations!
