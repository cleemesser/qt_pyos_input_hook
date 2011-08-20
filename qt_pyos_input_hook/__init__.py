import sys,os
from ctypes import *
basedir = os.path.dirname(__file__)
if sys.platform[:5] == 'linux':
    pyos = cdll.LoadLibrary(os.path.join(basedir, "libqt_pyos_input_hook.so"))
    qt_pyos_restore_input_hook = pyos.qt_pyos_restore_input_hook
    qt_pyos_disable_input_hook = pyos.qt_pyos_disable_input_hook
    
else:
    raise NotImplemented, "Need to write support for platforms other than linux"


