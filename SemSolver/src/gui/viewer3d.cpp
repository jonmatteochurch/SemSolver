#include "viewer3d.hpp"

/*
void Viewer3D::plotFunction(
        const SemSolver::Function<SemSolver::Point<2, double>, double> *function,
        const double &xmin,
        const double &ymin,
        const double &zmin,
        const double &xmax,
        const double &ymax,
        const double &zmax,
        const int &xdiv,
        const int &ydiv)
{
    Function f3d(function, this);

    f3d.setMesh(xdiv, ydiv);
    f3d.setDomain(xmin, xmax, ymin, ymax);
    f3d.setMinZ(zmin);
    f3d.setMaxZ(zmax);
    f3d.create();

    updateData();
    updateGL();
};
*/
void Viewer3D::plotFunction(
        const SemSolver::Function<SemSolver::Point<2, double>, double> *function,
        const double &xmin,
        const double &ymin,
        const double &xmax,
        const double &ymax,
        const int &xdiv,
        const int &ydiv)
{
    Qwt3D::Triple **data = new Qwt3D::Triple *[xdiv+1];
    double x, y, z;
    for(int i=0; i<=xdiv; ++i)
    {
        data[i] = new Qwt3D::Triple[ydiv+1];
        x = xmin + i*(xmax-xmin)/xdiv;
        for(int j=0; j<=ydiv; ++j)
        {
            y = ymin + j*(ymax-ymin)/ydiv;
            z = function->evaluate(SemSolver::Point<2, double>(x,y));
            data[i][j].x = x;
            data[i][j].y = y;
            data[i][j].z = z;
        }
    }
    loadFromData(data,xdiv+1,ydiv+1);
    updateData();
    updateGL();
    for(int i=0; i<=xdiv; ++i)
        delete[] data[i];
    delete[] data;
};
