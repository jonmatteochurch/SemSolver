include (../../SemSolver.config)
TARGET = SemSolver-GUI
TEMPLATE = app
QT += opengl \
    script
INCLUDEPATH += ../../include \
    $$BOOST_INCLUDE_DIR \
    $$CGAL_INCLUDE_DIR \
    $$TNT_INCLUDE_DIR \
    $$JAMA_INCLUDE_DIR \
    $$LIBARCHIVE_INCLUDE_DIR \
    $$QWT_INCLUDE_DIR \
    $$QWTPLOT3D_INCLUDE_DIR \
    $$QTMMLWIDGET_INCLUDE_DIR
LIBS += -lmpfr \
 -L$$CGAL_LIB_DIR -lCGAL -lCGAL_Core -Wl,-rpath,$$CGAL_LIB_DIR \
 -L$$LIBARCHIVE_LIB_DIR -larchive -Wl,-rpath,$$LIBARCHIVE_LIB_DIR \
 -L$$QWT_LIB_DIR -lqwt-qt4 -Wl,-rpath,$$QWT_LIB_DIR \
 -L$$QWTPLOT3D_LIB_DIR -lqwtplot3d-qt4 -Wl,-rpath,$$QWTPLOT3D_LIB_DIR \
 -L$$QTMMLWIDGET_LIB_DIR -lQtSolutions_MMLWidget-2.4 -Wl,-rpath,$$QTMMLWIDGET_LIB_DIR
LIBS += -L../../lib -lSemSolver-IO
OBJECTS_DIR = ../../obj/gui
MOC_DIR = moc
DESTDIR = ../../bin
HEADERS += viewer3d.hpp \
    viewer2d.hpp \
    removeparametersdialog.hpp \
    removegeometrydialog.hpp \
    removeequationdialog.hpp \
    removeboundaryconditionsdialog.hpp \
    openworkspacedialog.hpp \
    newworkspacedialog.hpp \
    newparametersdialog.hpp \
    newgeometryfrompslgdialog.hpp \
    newgeometrydialog.hpp \
    newequationdialog.hpp \
    newboundaryconditionsdialog.hpp \
    menubar.hpp \
    mainwindow.hpp \
    mainframe.hpp \
    importparametersfromworkspacedialog.hpp \
    importparametersdialog.hpp \
    importgeometryfromworkspacedialog.hpp \
    importgeometrydialog.hpp \
    importequationfromworkspacedialog.hpp \
    importequationdialog.hpp \
    importboundaryconditionsfromworkspacedialog.hpp \
    importboundaryconditionsdialog.hpp \
    geometrytolerancedialog.hpp \
    formula.hpp \
    exportsolutiondialog.hpp \
    exportplotdialog.hpp \
    exportparametersdialog.hpp \
    exportgeometrydialog.hpp \
    exportequationdialog.hpp \
    exportboundaryconditionsdialog.hpp \
    dock.hpp
SOURCES += viewer3d.cpp \
    viewer2d.cpp \
    removeparametersdialog.cpp \
    removegeometrydialog.cpp \
    removeequationdialog.cpp \
    removeboundaryconditionsdialog.cpp \
    openworkspacedialog.cpp \
    newworkspacedialog.cpp \
    newparametersdialog.cpp \
    newgeometryfrompslgdialog.cpp \
    newgeometrydialog.cpp \
    newequationdialog.cpp \
    newboundaryconditionsdialog.cpp \
    menubar.cpp \
    mainwindow.cpp \
    mainframe.cpp \
    main.cpp \
    importparametersfromworkspacedialog.cpp \
    importparametersdialog.cpp \
    importgeometryfromworkspacedialog.cpp \
    importgeometrydialog.cpp \
    importequationfromworkspacedialog.cpp \
    importequationdialog.cpp \
    importboundaryconditionsfromworkspacedialog.cpp \
    importboundaryconditionsdialog.cpp \
    geometrytolerancedialog.cpp \
    exportsolutiondialog.cpp \
    exportplotdialog.cpp \
    exportparametersdialog.cpp \
    exportgeometrydialog.cpp \
    exportequationdialog.cpp \
    exportboundaryconditionsdialog.cpp \
    dock.cpp
DEFINES += SEMDEBUG
