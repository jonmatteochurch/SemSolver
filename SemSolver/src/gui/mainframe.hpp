#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <qwt3d_types.h>

#include "../lib/semsolver/function.hpp"
#include "../lib/semsolver/point.hpp"
#include "../lib/semsolver/semgeometry.hpp"
#include "../lib/semsolver/semparameters.hpp"

#include "formula.hpp"
#include "viewer2d.hpp"
#include "viewer3d.hpp"

class MainFrame
    : public QTabWidget
{
    // interface
    QWidget     *problem;
    QVBoxLayout *problem_layout;
    QTabWidget  *equation_tab;
    QTabWidget  *boundary_conditions_tab;
    QTabWidget  *parameters_tab;
    QTabWidget  *geometry_tab;
    QWidget     *equation;
    QWidget     *boundary_conditions;
    QWidget     *parameters;
    QHBoxLayout *equation_layout;
    QHBoxLayout *boundary_conditions_layout;
    QHBoxLayout *parameters_layout;
    Formula     *equation_formula;
    QLabel      *border_label;
    QLabel      *condition_label;
    Formula     *boundary_condition;
    QLabel      *degree_label;
    QLabel      *degree;
    QLabel      *tolerance_label;
    QLabel      *tolerance;
    QLabel      *penality_label;
    QLabel      *penality;
    Viewer2D    *geometry_viewer;
    QWidget     *solution;
    QVBoxLayout *solution_layout;
    Viewer3D    *solution_viewer;

    QStringList bcs_mmls;

public:
    QComboBox   *borders;

    MainFrame(QWidget *parent = 0);
    ~MainFrame();

    inline void plotGeometry(SemSolver::SemGeometry<2, double> const &geometry)
    {
        geometry_viewer->plotGeometry(geometry);
    };

    inline void displayEquation(const QString &mml)
    {
        equation_formula->setContent(mml);
        equation_formula->setFixedSize(equation_formula->sizeHint());
    };

    void setBoundaryConditions(const QStringList &labels,
                               const QStringList &mmls);
    void displayBoundaryCondition(const int &index);
    inline void displayParameters(const SemSolver::SemParameters<double> &parameters)
    {
        degree->setText(QString::number(parameters.degree()));
        tolerance->setText(QString::number(parameters.tolerance()));
        penality->setText(QString::number(parameters.penality()));
    };

    inline void plotSolution(const SemSolver::Function< SemSolver::Point<2, double>, double> *u,
                             const double &xmin,
                             const double &ymin,
                             const double &xmax,
                             const double &ymax)
    {
        solution_viewer->plotFunction(u,xmin,ymin,xmax,ymax,20,20);
        setTabEnabled(1,true);
    };

    inline void plotSolution(const Qwt3D::TripleField &data, const Qwt3D::CellField &poly)
    {
        solution_viewer->loadFromData(data, poly);
        solution_viewer->updateData();
        solution_viewer->updateGL();
        setTabEnabled(1,true);
    };

    inline void resetGeometry()
    {
        geometry_viewer->clearPlot();
    };

    inline void resetEquation()
    {
        equation_formula->setText("none");
        equation_formula->setFixedSize(equation_formula->sizeHint());
    };

    void resetBoundaryConditions();

    inline void resetParameters()
    {
        degree->setText("none");
        tolerance->setText("none");
        penality->setText("none");
    };

    inline void resetSolution()
    {
        setTabEnabled(1, false);
        setCurrentIndex(0);
        solution_layout->removeWidget(solution_viewer);
        delete solution_viewer;
        solution_viewer = new Viewer3D;
        solution_layout->addWidget(solution_viewer);
    };

    inline bool exportSolution(const QString &fileName, const QString &format)
    {
        return solution_viewer->save(fileName, format);
    };
};

#endif // MAINFRAME_HPP
