#ifndef VIEWER3D_HPP
#define VIEWER3D_HPP

#include <QWidget>

#include <qwt3d_surfaceplot.h>

#include "../lib/semsolver/point.hpp"
#include "../lib/semsolver/function.hpp"

class Viewer3D
    : public Qwt3D::SurfacePlot
{
public:
    inline Viewer3D(QWidget *parent = 0)
        : Qwt3D::SurfacePlot(parent)
    {
    };
    void plotFunction(const SemSolver::Function<SemSolver::Point<2, double>, double> *function,
                      double const &xmin,
                      double const &ymin,
                      double const &xmax,
                      double const &ymax,
                      int const &xdiv,
                      int const &ydiv);
};

#endif // VIEWER3D_HPP
