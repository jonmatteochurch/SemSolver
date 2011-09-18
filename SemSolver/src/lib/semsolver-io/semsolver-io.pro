include (../../../SemSolver.config)
QT += script
TARGET = SemSolver-IO
TEMPLATE = lib
CONFIG += static
HEADERS += workspace.hpp \
    subdomains.hpp \
    pslg.hpp \
    parameters.hpp \
    nextnonemptlinevalues.hpp \
    geometry.hpp \
    equation.hpp \
    carchive \
    boundaryconditions.hpp \
    archive.hpp
SOURCES += workspace.cpp \
    nextnonemptlinevalues.cpp \
    geometry.cpp \
    archive.cpp
OBJECTS_DIR = ../../../obj/io
DESTDIR = ../../../lib
INCLUDEPATH += ../../../include \
    $$TNT_INCLUDE_DIR
DEFINES += SEMDEBUG
