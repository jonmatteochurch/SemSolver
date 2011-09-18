# SemSolver
Project for **Advanced Programming for Scientific Computing** (PACS) and **Numerical Analysis for Partial Differential Equations 2** (ANEDP2)

SEM-NI (Spectral Elements Method with Numerical Integration) Solver for elliptic problems on generic polygonal domains that may present holes (2D).
The software provides both a library and a GUI to allow its use both standalone and as a component.
All the code provided is portable and can run on Linux, Mac, and Windows.

## DEPENDENCIES
- **Boost** (1.47.0)
- **CGAL** (3.8)
- **TNT/Jama** (1.2.6 / 1.2.5)
- **Libarchive** (2.8.4)
- **Qt** (4.7.2)
- **Qwt** (6.0.1)
- **QwtPlot3D** (0.2.7)
- **QtMmlWidget** (2.4.1)

(in parenthesis is the tested version)

 ##  CONFIGURE

### On Unix
Set variables in [SemSolver.config](./SemSolver/SemSolver.config) for matching your  build environment.

### On Windows

Set solution properties within **Visual Studio** (2008) so that all  dependencies will  be
found.
By default the following environment variables are used
 - BOOST_INCLUDE_DIR
 - BOOST_LIB_DIR
 - CGAL_INCLUDE_DIR
 - CGAL_LIB_DIR
 - TNT_INCLUDE_DIR
 - JAMA_INCLUDE_DIR
 - LIBARCHIVE_INCLUDE_DIR
 - LIBARCHIVE_LIB_DIR
 - QWT_INCLUDE_DIR
 - QWT_LIB_DIR
 - QWTPLOT3D_INCLUDE_DIR
 - QWTPLOT3D_LIB_DIR
 - QTMMLWIDGET_INCLUDE_DIR
 - QTMMLWIDGET_LIB_DIR

## BUILD

### On Uinx

Run on command window
```
$ qmake
$ make
```

## On Windows

Select **Build All** from **Build** menu.
