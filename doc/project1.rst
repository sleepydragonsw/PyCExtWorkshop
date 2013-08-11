Project 1: Compile Python Interpreter
=====================================

Introduction
------------

Although it is not strictly required to develop Python C extension modules,
it is very helpful to first build the Python interpreter for yourself.

The first reason is that it helps work through the same problems
that tend to arise when compiling extension modules,
most notably, setting up the build envionment.
The second reason is that it is far easier to debug extension modules
when the entire Python interpreter itself has been compiled from source.
It allows you to "step into" the Python interpreter
when debugging your extension module
and understand the context from which your extension module is being called.

Get Source Code
---------------

There are several different ways to get the source code for the Python interpreter.
The instructions in this workshop assume the source code of Python version 2.7.5.
To get the source code, download the archive from http://www.python.org/download.

Compiling on Mac/Linux
----------------------

Since Mac and Linux generally ship with C compilers,
run the following commands to build.

#. ``./configure --with-pydebug``
#. ``make``

These instructions are adapted from http://docs.python.org/devguide.

Compiling on Windows with Visual Studio 2008
--------------------------------------------

If using Visual Studio 2008 on Windows,
this version of the source code will compile right out-of-the-box.

#. Open ``PCBuild\\pcbuild.sln`` in Visual Studio
#. Select "Debug" in the drop-down box near the top of the screen
#. Select "Win32" in the drop-down box near the top of the screen
#. Press F7 to compile

This will create ``PCBuild\\python_d.exe``,
a debug version of the Python interpreter.

Compiling on Windows with Visual Studio 2010
--------------------------------------------

If using Visual Studio 2010 on Windows,
you can get a slightly-modified version of the source code
that is tweaked to work with Visual Studio 2010.

#. Clone the Git repository at https://github.com/sleepydragonsw/Python-2.7.5-VS2010
#. Launch "Visual Studio Command Prompt (2010)" from the Start menu
#. Run ``build32-debug.bat``

Your self-compiled Python interpreter will be located at
``Python-2.7.5\\PCbuild\\python_d.exe``.
