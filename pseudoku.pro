# -------------------------------------------------
# Project created by QtCreator 2009-11-18T13:16:15
# -------------------------------------------------
TARGET = pseudoku
TEMPLATE = app
LIBS += -L \
    /usr/local/lib
SOURCES += qmain.cpp \
    mainwindow.cpp \
    qboardmodel.cpp \
    cell.cpp \
    board.cpp \
    tools.cpp \
    qboarddelegate.cpp
HEADERS += mainwindow.h \
    qboardmodel.h \
    cell.h \
    board.h \
    tools.h \
    qboarddelegate.h \
    local.h
MAKEFILE = Makefile
FORMS += mainwindow.ui
QMAKE_CXXFLAGS_RELEASE += -fopenmp
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -fopenmp \
    -g
QMAKE_LFLAGS_DEBUG += -fopenmp
