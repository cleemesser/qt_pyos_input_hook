==================
qt_pyos_input_hook
==================
[as posted to the PySide mailing lst]

I've been using PySide in the latest enthought python distribution (7.1-2) and on
my ubuntu laptop using the pyside PPA. I do interactive signal analysis and would
like to incorporate pyside into my programs along with ipython. One feature that
is quite nice about Tkinter and PyQt is that you can continue to run and interact
with python at the command line while running gui widgets.

When I try this from python's interpreter or from ipython (version 0.11) with
pyside, the interpreter blocks while the gui elements are active. I looked into
why by digging through the ipython code and it looks like it tries to invoke
something called QtCore.pyqtRestoreInputHook(). This doesn't exist in PySide.

Looking at the _tkinter.c code, it seems this is done via python's C-API using a
pointer, PyOS_InputHook. I'd never heard of it before, but the way it appears to
work, is that while python is waiting for text input, it calls this function to
do idle processing--allowing for gui event processing during these idle times.

I was hoping that PySide could include support for this. Perhaps it already does
but I didn't find it?

In an effort to get things working in my use case and to be helpful to the
greater python/pyside cause, I have been experimenting with a prototype.

Just as warning, it is written starting from a state of profound ignorance: 
I have only vague memories of Qt's and python's threading model and this is first
use of Qt's processing events. I've made a couple of guesses about what QCoreApplication
and processEvents did.

Surprisingly, after some experimentation, my prototype appears to work, at least
in a few individual cases I've tried.

My development platform is python 2.7 on linux x86_64 (ubuntu 10.10) using pyside
1.05. I linked it to Qt 4.7.3 and libpython2.7 but doesn't depend on the pyside
code, just Qt. If I knew how to use ctypes to access C++ libraries,
it probably wouldn't even require the compilation step.

ipython uses git, so I've put the repo at http://github.com/cleemesser/qt_pyos_input_hook


How I'm getting started
-----------------------
This isn't meant to be used by non-developers, but here are my notes for myself
about how I got this working my development machine ubuntu 10.10.

1. edit qt_pyos_input_hook/qt_pyos_input_hook.pro  at these lines::
    INCLUDEPATH += /home/clee/py27env/py27mkl/include/python2.7
    LIBS += -L/home/clee/py27env/lib  -lpython2.7

so that the include path points to where your own python include files and library can be found. I'm using a custom compiled python2.7.

2. build the library::

    cd qt_pyos_input_hook

    qmake-qt4
    make
    cd ..

That should produce a library like libqt_pyos_input_hook.so.1.0.0

3. install as a developer::
    python setupegg.py develop

4. test application
I don't have this automated yet, but you can type for yourself::
    cd tests
    ipython -i test_ipython_gui.py

This does the equivalent of running from ipython
%gui qt
from qt_pyos_input_hook import *
qt_pyos_restore_input_hook()
run gui-qt.py

# at this point I was able to see a functiong gui window with a close button while I was still able to interact at the command line.

Here's what my session looked like::

    In [1]: %gui qt
    Out[1]: <PySide.QtGui.QApplication at 0x1b99c68>

    In [2]: from qt_pyos_input_hook import *

    In [3]: qt_pyos_restore_input_hook()
    Out[3]: -1495714920

    In [4]: run gui-qt.py
    trying to start event loop

    In [5]: 


ipython integration
-------------------

I've included a small experimental patch to ipython-0.11 to integrate the pyside
inputhook so it is used automatically.

    
How it works (I think)
----------------------


qt_pyos_restore_input_hook()
----------------------------
sets the python input hook to a function which gets the current application Qt 


qt_pyos_disable_input_hook()
----------------------------
unsets PyOS_Input_Hook
