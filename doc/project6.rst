Project 6: Calculating PNG Image Size Using libpng
==================================================

Introduction
------------

In this project you will "fill in the blanks" of a function
that uses the open-source PNG image library "libpng"
to determine the height and width of a PNG image.
This function is fully robust and handles all errors.

You will need to use all knowledge learned up to this point
in order to complete this project,
along with some new things that will be introduced,
such as raising exceptions, memory management, and using third-party DLLs.


Compile libpng
--------------

In this step you will compile libpng for yourself.

  1. Download the source code for the latest version of libpng
     (which at the time of writing is 1.6.6)
     from http://libpng.org/pub/png/libpng.html.
  2. Extract the downloaded file
     (in my case ``lpng166.zip``)
     somewhere on your computer.
  3. Open ``projects\vstudio\vstudio.sln`` in Visual Studio 2010
  4. In the drop-down listbox at the top of the screen
     change "Release" to "Debug Library"
     (see screenshot below)
  5. Press F7 to compile libpng; this should take less than 5 minutes

.. image:: images/BuildConfigScreenshot.png

If successful, the file ``projects\vstudio\Debug\libpng16.dll``
will have been created.


png_dimensions() Function Skeleton
----------------------------------

Below is an implementation of a Python C extension function
that reads the width and height (in pixels) of a PNG image.
However, there are lots of "holes" in it
that you will be implementing in the following sections.

For now, copy and paste this code into the C source file
for your extension module (eg. ``denver.c``):

.. code-block:: c

    #include <png.h>

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
                // TODO: set exception from errno
            } else {
                // TODO: set exception due to premature end-of-file
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
            // TODO: set exception with the given message
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

        // TODO: replace the hardcoded path with an argument to the function (hint: PyArg_ParseTuple)
        path = "test.png";

        f = fopen(path, "rb");
        if (!f) {
            // TODO: set exception from errno
            Py_RETURN_NONE;
        }

        n = fread(buf, 1, 8, f);
        if (n == 0) {
            fclose(f);
            // TODO: set exception from errno
            Py_RETURN_NONE;
        }

        isPng = png_check_sig(buf, n);
        if (! isPng) {
            fclose(f);
            // TODO: raise a ValueError whose message indicates invalid PNG signature
            Py_RETURN_NONE;
        }

        png_ptr = png_create_read_struct(
            PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (! png_ptr) {
            fclose(f);
            // TODO: raise exception using the special "no memory" function
            Py_RETURN_NONE;
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (! info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            fclose(f);
            // TODO: raise exception using the special "no memory" function
            Py_RETURN_NONE;
        }

        if (setjmp(png_jmpbuf(png_ptr)) != 0) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(f);
            // TODO: return NULL once all error handling is implemented
            Py_RETURN_NONE;
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
        // TODO: handle out-of-memory error

        heightObj = PyInt_FromLong(image_height);
        // TODO: handle out-of-memory error; make sure to use Py_DECREF
        // for any newly-created Python objects created above

        // TODO: create a tuple containing the width and heigh objects
        // and return it
        Py_RETURN_NONE;
    }


Add png_dimensions() to Function Table
--------------------------------------

