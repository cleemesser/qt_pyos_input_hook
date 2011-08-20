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


Getting started
---------------

Doing things by hand on ubuntu 10.10 (chris)

cd qt_pyos_input_hook
qmake-qt4
make 