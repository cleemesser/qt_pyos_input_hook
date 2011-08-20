# IPython log file

get_ipython().magic(u"gui qt")
from qt_pyos_input_hook import *
qt_pyos_restore_input_hook()
get_ipython().magic(u"run gui-qt.py")
# example of continuing interaction
get_ipython().system(u"ls -F --color ")
