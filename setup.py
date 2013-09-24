# -*- coding: utf-8 -*-

import distutils.core

ext_module = distutils.core.Extension(
    "denver",
    sources=["denver.c"],
    include_dirs=["c:\\dev\\temp\\lpng166"],
    library_dirs=["C:\\dev\\temp\\lpng166\\projects\\vstudio\\Debug"],
    libraries=["libpng16"],
)

distutils.core.setup(
    ext_modules=[ext_module],
)
