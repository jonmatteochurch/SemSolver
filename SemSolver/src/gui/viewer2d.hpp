#ifndef VIEWER2D_HPP
#define VIEWER2D_HPP

#include <cmath>

#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_rescaler.h>

#include "../lib/semsolver/point.hpp"
#include "../lib/semsolver/polygonation.hpp"
#include "../lib/semsolver/pslg.hpp"
#include "../lib/semsolver/semgeometry.hpp"

class Viewer2D
    : public QwtPlot
{
    typedef std::map<int, QwtPlotMarker*> MarkersMap;
    typedef std::map<int, QwtPlotCurve*>  CurvesMap;

    // interface
    QwtPlotRescaler *rescaler;

    // variables
    MarkersMap vertices;
    CurvesMap  segments;
    MarkersMap segment_markers;
    MarkersMap holes;
    CurvesMap  elements;
    MarkersMap element_markers;

public:

    Viewer2D(QWidget *parent = 0);
    ~Viewer2D();

    // modifying methods
    void addVertex(int const &id,
                   double const &x,
                   double const &y);
    void addSegment(int const &id,
                    SemSolver::Point<2, double> const &source,
                    SemSolver::Point<2, double> const &target);
    void addHole(int const &id,
                 double const &x,
                 double const &y);
    void addQuadrangle(int const &id,
                       SemSolver::Point<2, double> const &p0,
                       SemSolver::Point<2, double> const &p1,
                       SemSolver::Point<2, double> const &p2,
                       SemSolver::Point<2, double> const &p3);

    void modifyVertex(int const &id,
                      double const &x,
                      double const &y);
    void modifySegment(int const &id,
                       SemSolver::Point<2, double> const &source,
                       SemSolver::Point<2, double> const &target);
    void modifyHole(int const &id,
                    double const &x,
                    double const &y);

    void removeVertex(int const &id);
    void removeSegment(int const &id);
    void removeHole(int const &id);

    void plotGeometry(const SemSolver::SemGeometry<2, double> &geometry);
    void plotPSLG(const SemSolver::PSLG<double> &pslg);
    void plotSubDomains(const SemSolver::Polygonation<2, double> &subdomains);

    void clearPlot();

    void updateVisibleArea();
};

#endif // VIEWER2D_HPP
