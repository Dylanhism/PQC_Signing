from pyspx.bindings import PySPXBindings as _PySPXBindings
from _spx_shake_128f import ffi as _ffi, lib as _lib

import sys

sys.modules[__name__] = _PySPXBindings(_ffi, _lib)
