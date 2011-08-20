#-------------------------------------------------
#
# Project created by QtCreator 2011-08-19T18:52:57
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = qt_pyos_input_hook
CONFIG   += dll

# CONFIG   -= app_bundle


TEMPLATE = lib
INCLUDEPATH += /home/clee/py27env/py27mkl/include/python2.7

SOURCES += qt_pyos_input_hook.cpp
LIBS += -L/home/clee/py27env/lib  -lpython2.7
