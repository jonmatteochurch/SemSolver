#include "viewer2d.hpp"

#include "../lib/semsolver/polygonwithholes.hpp"

Viewer2D::Viewer2D(QWidget *parent)
    : QwtPlot(parent)
{
    rescaler = new QwtPlotRescaler(this->canvas());
    this->setCanvasLineWidth(1);
    this->setFrameStyle(QwtPlot::StyledPanel);
    this->setCanvasBackground(Qt::white);
    this->enableAxis(0,true);
    this->enableAxis(1,false);
    this->enableAxis(2,true);
    this->enableAxis(3,false);
    rescaler->setReferenceAxis(QwtPlot::xBottom);
    rescaler->setAspectRatio(QwtPlot::yLeft, 1.0);
    rescaler->setAspectRatio(QwtPlot::yRight, 0.0);
    rescaler->setAspectRatio(QwtPlot::xTop, 0.0);
#if QWT_VERSION < 0x060000
    rescaler->setIntervalHint(0, QwtDoubleInterval(-1., 1.));
    rescaler->setIntervalHint(2, QwtDoubleInterval(-1., 1.));
#else
    rescaler->setIntervalHint(0, QwtInterval(-1., 1.));
    rescaler->setIntervalHint(2, QwtInterval(-1., 1.));
#endif
	rescaler->setRescalePolicy(QwtPlotRescaler::Fitting);
    rescaler->setEnabled(true);
    rescaler->setExpandingDirection(QwtPlotRescaler::ExpandBoth);
    rescaler->rescale();
    this->replot();
};

Viewer2D::~Viewer2D()
{
    for(MarkersMap::const_iterator it = vertices.begin(); it != vertices.end(); ++it)
        delete it->second;
    for(CurvesMap::const_iterator it = segments.begin(); it != segments.end(); ++it)
        delete it->second;
    for(MarkersMap::const_iterator it = segment_markers.begin();
    it != segment_markers.end(); ++it)
        delete it->second;
    for(MarkersMap::const_iterator it = holes.begin(); it != holes.end(); ++it)
        delete it->second;
    for(CurvesMap::const_iterator it = elements.begin(); it != elements.end(); ++it)
        delete it->second;
    for(MarkersMap::const_iterator it = element_markers.begin();
    it != element_markers.end(); ++it)
        delete it->second;
    delete rescaler;
};

void Viewer2D::addVertex(int const &id,
                         double const &x,
                         double const &y)
{
    QwtPlotMarker *marker = new QwtPlotMarker;
    marker->setLabel("V" + QString::number(id));
    marker->setLabelAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    marker->setValue(x, y);
    marker->attach(this);
    vertices.insert(std::pair<int, QwtPlotMarker*>(id, marker));
    this->replot();
    updateVisibleArea();
};

void Viewer2D::addSegment(int const &id,
                          SemSolver::Point<2, double> const &source,
                          SemSolver::Point<2, double> const &target)
{
    QVector<double> x, y;
    x.push_back(source.x());
    x.push_back(target.x());
    y.push_back(source.y());
    y.push_back(target.y());
    QwtPlotCurve *segment = new QwtPlotCurve();
#if QWT_VERSION < 0x060000
    segment->setData(x, y);
#else 
	segment->setSamples(x, y);
#endif
    segment->setPen(QPen(Qt::black));
    segment->setStyle(QwtPlotCurve::Lines);
    segment->setRenderHint(QwtPlotItem::RenderAntialiased);
    segment->attach(this);
    QwtPlotMarker *marker = new QwtPlotMarker();
    marker->setLabel("S" + QString::number(id));
    marker->setLabelAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    marker->setValue((x[0]+x[1])/2,(y[0]+y[1])/2);
    marker->attach(this);
    segments[id] = segment;
    segment_markers[id] = marker;
    this->replot();
    updateVisibleArea();
};

void Viewer2D::addHole(int const &id,
                       double const &x,
                       double const &y)
{
    QwtPlotMarker *marker = new QwtPlotMarker;
    marker->setLabel("H" + QString::number(id));
    marker->setLabelAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    marker->setValue(x,y);
    marker->attach(this);
    holes.insert(std::pair<int,QwtPlotMarker*>(id,marker));
    this->replot();
    updateVisibleArea();
};

void Viewer2D::addQuadrangle(int const &id,
                             SemSolver::Point<2, double> const &p0,
                             SemSolver::Point<2, double> const &p1,
                             SemSolver::Point<2, double> const &p2,
                             SemSolver::Point<2, double> const &p3)
{
    QVector<double> x, y;
    x.push_back(p0.x());
    x.push_back(p1.x());
    x.push_back(p2.x());
    x.push_back(p3.x());
    x.push_back(p0.x());
    y.push_back(p0.y());
    y.push_back(p1.y());
    y.push_back(p2.y());
    y.push_back(p3.y());
    y.push_back(p0.y());
    QwtPlotCurve *quadrangle = new QwtPlotCurve();
#if QWT_VERSION < 0x060000
    quadrangle->setData(x, y);
#else 
	quadrangle->setSamples(x, y);
#endif
    quadrangle->setPen(QPen(Qt::gray));
    quadrangle->setStyle(QwtPlotCurve::Lines);
    quadrangle->setRenderHint(QwtPlotItem::RenderAntialiased);
    quadrangle->attach(this);
    QwtPlotMarker *marker = new QwtPlotMarker();
    marker->setLabel("E"+QString::number(id));
    marker->setLabelAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    marker->setValue((x[0]+x[1]+x[2]+x[3])/4,(y[0]+y[1]+y[2]+y[3])/4);
    marker->attach(this);
    elements.insert(std::pair<int, QwtPlotCurve*>(id, quadrangle));
    element_markers.insert(std::pair<int, QwtPlotMarker*>(id, marker));
    this->replot();
    updateVisibleArea();
};

void Viewer2D::modifyVertex(int const &id,
                            double const &x,
                            double const &y)
{
    vertices[id]->setValue(x, y);
    this->replot();
    updateVisibleArea();
};

void Viewer2D::modifySegment(int const &id,
                             SemSolver::Point<2, double> const &source,
                             SemSolver::Point<2, double> const &target)
{
    QVector<double> x, y;
    x.push_back(source.x());
    x.push_back(target.x());
    y.push_back(source.y());
    y.push_back(target.y());
#if QWT_VERSION < 0x060000
    segments[id]->setData(x, y);
#else 
	segments[id]->setSamples(x, y);
#endif
    segment_markers[id]->setValue((x[0]+x[1])/2,(y[0]+y[1])/2);
    this->replot();
    updateVisibleArea();
};

void Viewer2D::modifyHole(int const &id,
                          double const &x,
                          double const &y)
{
    holes[id]->setValue(x, y);
    this->replot();
    updateVisibleArea();
};

void Viewer2D::removeVertex(int const &id)
{
    vertices[id]->detach();
    delete vertices[id];
    vertices.erase(id);
    this->replot();
    updateVisibleArea();
};

void Viewer2D::removeSegment(int const &id)
{
    segments[id]->detach();
    delete segments[id];
    segments.erase(id);
    segment_markers[id]->detach();
    delete segment_markers[id];
    segment_markers.erase(id);
    this->replot();
    updateVisibleArea();
};

void Viewer2D::removeHole(int const &id)
{
    holes[id]->detach();
    delete holes[id];
    holes.erase(id);
    this->replot();
    updateVisibleArea();
};

void Viewer2D::plotGeometry(const SemSolver::SemGeometry<2, double> &geometry)
{
    clearPlot();
    plotSubDomains(geometry.subDomains());
    plotPSLG(geometry.domain());
};

void Viewer2D::plotPSLG(const SemSolver::PSLG<double> &pslg)
{
    SemSolver::PSLG<double>::Segment segment;
    SemSolver::PSLG<double>::Vertex const *source, *target;
    for(uint i=0; i<pslg.segments(); ++i)
    {
        segment = pslg.segment(i);
        for(source=&(pslg.vertex(0)); source->number!=segment.source; ++source);
        for(target=&(pslg.vertex(0)); target->number!=segment.target; ++target);
        addSegment(segment.number,
                   SemSolver::Point<2,double>(source->x,source->y),
                   SemSolver::Point<2,double>(target->x,target->y));
    }
};

void Viewer2D::plotSubDomains(const SemSolver::Polygonation<2,double> &subdomains)
{
    for(uint i=0; i<subdomains.size(); ++i)
    {
        SemSolver::Polygonation<2, double>::Element element = subdomains.element(i);
        addQuadrangle(i+1, element.vertex(0), element.vertex(1), element.vertex(2),
                      element.vertex(3));
    }
};

void Viewer2D::clearPlot()
{
    for(MarkersMap::const_iterator it = vertices.begin(); it != vertices.end(); ++it)
        delete it->second;
    for(CurvesMap::const_iterator it = segments.begin(); it != segments.end(); ++it)
        delete it->second;
    for(MarkersMap::const_iterator it = segment_markers.begin();
    it != segment_markers.end(); ++it)
        delete it->second;
    for(MarkersMap::const_iterator it = holes.begin(); it != holes.end(); ++it)
        delete it->second;
    for(CurvesMap::const_iterator it = elements.begin(); it != elements.end(); ++it)
        delete it->second;
    for(MarkersMap::const_iterator it = element_markers.begin();
    it != element_markers.end(); ++it)
        delete it->second;
    vertices.clear();
    segments.clear();
    segment_markers.clear();
    holes.clear();
    elements.clear();
    element_markers.clear();

	this->detachItems(QwtPlotItem::Rtti_PlotCurve);
	this->detachItems(QwtPlotItem::Rtti_PlotMarker);
	this->replot();
    this->updateVisibleArea();
};

void Viewer2D::updateVisibleArea()
{
    double x_min = 0, x_max = 0, y_min = 0, y_max = 0;
    MarkersMap::const_iterator vit = vertices.begin(), hit = holes.begin();
    CurvesMap::const_iterator sit = segments.begin(), eit = elements.begin();
    if(vertices.size())
    {
        x_min = x_max = vit->second->xValue();
        y_min = y_max = vit->second->yValue();
    }
    else if(segments.size())
    {
        x_min = sit->second->minXValue();
        x_max = sit->second->maxXValue();
        y_min = sit->second->minYValue();
        y_max = sit->second->maxYValue();
    }
    else if(holes.size())
    {
        x_min = x_max = hit->second->xValue();
        y_min = y_max = hit->second->yValue();
    }
    else if(elements.size())
    {
        x_min = eit->second->minXValue();
        x_max = eit->second->maxXValue();
        y_min = eit->second->minYValue();
        y_max = eit->second->maxYValue();
    }
    if(vertices.size() > 1)
    {
        while(++vit != vertices.end())
        {
            double x, y;
            x = vit->second->xValue();
            y = vit->second->yValue();
            x_min = x<x_min ? x : x_min;
            x_max = x>x_max ? x : x_max;
            y_min = y<y_min ? y : y_min;
            y_max = y>y_max ? y : y_max;
        }
    }
    if(holes.size() > 1)
    {
        while(++hit != holes.end())
        {
            double x, y;
            x = hit->second->xValue();
            y = hit->second->yValue();
            x_min = x<x_min ? x : x_min;
            x_max = x>x_max ? x : x_max;
            y_min = y<y_min ? y : y_min;
            y_max = y>y_max ? y : y_max;
        }
    }
    if(segments.size() > 1)
    {
        while(++sit != segments.end())
        {
            double x0, x1, y0, y1;
            x0 = sit->second->minXValue();
            y0 = sit->second->minYValue();
            x1 = sit->second->maxXValue();
            y1 = sit->second->maxYValue();
            x_min = x0<x_min ? x0 : x_min;
            x_max = x1>x_max ? x1 : x_max;
            y_min = y0<y_min ? y0 : y_min;
            y_max = y1>y_max ? y1 : y_max;
        }
    }
    if(elements.size() > 1)
    {
        while(++eit != elements.end())
        {
            double x0, x1, y0, y1;
            x0 = eit->second->minXValue();
            y0 = eit->second->minYValue();
            x1 = eit->second->maxXValue();
            y1 = eit->second->maxYValue();
            x_min = x0<x_min ? x0 : x_min;
            x_max = x1>x_max ? x1 : x_max;
            y_min = y0<y_min ? y0 : y_min;
            y_max = y1>y_max ? y1 : y_max;
        }
    }
    double delta_x = x_max-x_min, delta_y = y_max-y_min;
    if (!delta_x)
        delta_x = 1;
    if (!delta_y)
        delta_y = 1;
#if QWT_VERSION < 0x060000
    rescaler->setIntervalHint(2, QwtDoubleInterval(x_min-.1*delta_x, x_max+.1*delta_x));
    rescaler->setIntervalHint(0, QwtDoubleInterval(y_min-.1*delta_y, y_max+.1*delta_y));
#else
    rescaler->setIntervalHint(2, QwtInterval(x_min-.1*delta_x, x_max+.1*delta_x));
    rescaler->setIntervalHint(0, QwtInterval(y_min-.1*delta_y, y_max+.1*delta_y));
#endif
	rescaler->rescale();
};

