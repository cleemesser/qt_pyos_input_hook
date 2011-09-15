#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QThread>
extern "C" {
#include "Python.h"
}


// don't know how the thread models interact so disable python thread macros for now
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

    Thinking about threading:

    In general, Qt gui elements can only be called from 
    the main thread this function is mean to process gui events so it doesn't
    make sense to call it from any other thread

    Do I need to protect it?

    The PyOS_InputHook is invoked from my_fgets in Python-2.7.2/Parser/myreadline.c
    so it happens when python is requesting a line of (usually interactive) input
    it runs the hook (if !NULL) and get executes a regular fgets(buf, len,fp).

    The hook runs repeatedly while sitting at the prompt and could be seen while I was pressing keys
    
  From Qt docs for QObject (http://doc.qt.nokia.com/stable/qobject.html) ::
  
     To move an object to the main thread, use QApplication::instance() to
   retrieve a pointer to the current application, and then use
   QApplication::thread() to retrieve the thread in which the application lives.
    
  */

  // Ok, I have no idea what needs to be done in terms of threads
  // in _tkinter.c this part is surounded by Py_BEGIN_ALLOW_THREADS

  // trying QCoreApplication after doing things with QApplication in case was using
  // a non gui
  
  QCoreApplication *app = QCoreApplication::instance(); 
   if (app != NULL) {
     // the python interpreter process with keyboard input needs to be in the

     // same thread that launched the Qt gui. Is there a possibility this isn't
     // true?
     // trying protection from from Qt side of things by getting QApplication
     // object and compare it's ->thread() with current thread
     QThread * current_thread_ptr =  QThread::currentThread();
     if (app->thread() != current_thread_ptr) {
       // fprintf(stderr, "whoa! trying to call qt PyOS_InputHook from wrong thread. return\n");
       return 0;
     } else {
     // this may not process DeferredDeleteEvents (tooltips messed up) !!!
       app->processEvents(QEventLoop::AllEvents, 100); //100ms timeout, is thread safe
       //  fprintf(stderr, "."); // for debugging to make sure it's being called
     }
   }
   return 0;
}

/* now lets try using the eventloop and a timer*/
static int qt_pyos_input_hook2(void)
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
     QTimer timer;
     timer.setInterval(90);
     QObject::connect(&timer, SIGNAL(timeout()), app, SLOT(quit()));
     timer.start();
     app->exec();
     timer.stop();
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

