# -*- coding: utf-8 -*-

import distutils.core

ext_module = distutils.core.Extension(
    "denver",
    sources=["denver.c"],
)

distutils.core.setup(
    ext_modules=[ext_module],
)
