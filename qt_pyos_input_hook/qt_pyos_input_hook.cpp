#include <QtCore/QCoreApplication>

extern "C" {
#include "Python.h"
}


// don't know how the thread models interact so disable for now
#undef WITH_THREAD
extern "C" {
  static int qt_pyos_input_hook(void);
  void qt_pyos_disable_input_hook(void);
  void qt_pyos_restore_input_hook(void);
}


static int qt_pyos_input_hook(void)
{
  /*
    not sure exactly what to do here, but know that once this is set, it is
    called to execute some gui related stuff for a time when python is waiting
    for input at the interactive prompt
    grab code from Modules/_tkinter.c for hints
  */

  // Ok, I have no idea what needs to be done in terms of threads
  // in _tkinter.c this part is surounded by Py_BEGIN_ALLOW_THREADS

   QCoreApplication *app = QCoreApplication::instance();
   if (app != NULL) {
     app->processEvents(QEventLoop::AllEvents, 100);
   }
   return 0;
}

void qt_pyos_restore_input_hook(void)
{
  if (PyOS_InputHook == NULL) {
    // in _tkinter.c this is surounded by threads macors
    PyOS_InputHook = qt_pyos_input_hook;
  }
}


void qt_pyos_disable_input_hook(void)
{
  if (PyOS_InputHook == qt_pyos_input_hook) {
    PyOS_InputHook = NULL;
  }
}

