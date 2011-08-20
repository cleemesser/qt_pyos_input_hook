import sys,os
from ctypes import *

if sys.platform[:5] == 'linux':
    pyos = cdll.LoadLibrary(os.path.join(basedir, "libqt_pyos_input_hook.so"))
    print pyos
    pyos.qt_pyos_restore_input_hook()
else:
    raise NotImplemented, "Need to write support for platforms other than linux"
