#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QFile>
#include <QMainWindow>
#include <QStatusBar>
#include <QString>
#include <QWidget>

#include <qwt3d_types.h>

#include "../lib/semsolver/function.hpp"
#include "../lib/semsolver/matrix.hpp"
#include "../lib/semsolver/problem.hpp"
#include "../lib/semsolver/semspace.hpp"
#include "../lib/semsolver/vector.hpp"

#include "dock.hpp"
#include "mainframe.hpp"
#include "menubar.hpp"

class MainWindow
    : public QMainWindow
{
    Q_OBJECT

    // interface
    MainFrame  *main_frame;
    Dock       *dock;
    MenuBar    *menu_bar;
    QStatusBar *status_bar;

    // files
    QFile       *workspace;
    QFile       *geometry;
    QFile       *equation;
    QFile       *boundary_conditions;
    QFile       *parameters;

    //variables
    SemSolver::Problem<2, double> *problem;
    SemSolver::SemSpace<2, double> *space;
    SemSolver::Matrix<double> problem_matrix;
    SemSolver::Vector<double> problem_vector;
    SemSolver::Vector<double> solution_vector;
    SemSolver::Function< SemSolver::Point<2, double>, double > *solution_function;
    Qwt3D::TripleField solution_data;
    Qwt3D::CellField solution_poly;
    double xmin, ymin, zmin, xmax, ymax, zmax;
    int plot_style;

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    bool loadWorkspace(QString const &workspace);

    void resetWorkspace();
    void resetGeometry();
    void resetEquation();
    void resetBoundaryConditions();
    void resetParameters();
    void resetSolution();

    void plotSolution();

public slots:

    void newWorkspace();
    void openWorkspace();

    void newGeometry();
    void newGeometryFromPslg();
    void importGeometry();
    void importGeometryFromWorkspace();
    void exportGeometry();
    void removeGeometry();

    void newEquation();
    void importEquation();
    void importEquationFromWorkspace();
    void exportEquation();
    void removeEquation();

    void newBoundaryConditions();
    void importBoundaryConditions();
    void importBoundaryConditionsFromWorkspace();
    void exportBoundaryConditions();
    void removeBoundaryConditions();

    void newParameters();
    void importParameters();
    void importParametersFromWorkspace();
    void exportParameters();
    void removeParameters();

    void solveLU();
    void solveQR();
    void solveCholesky();
    void exportSolution();
    void changePlotStyle();
    void exportPlot();

    inline void displayBoundaryCondition(const int &index)
		{
    main_frame->displayBoundaryCondition(index);
};

    bool loadGeometry(QString const &geometry);
    bool loadEquation(QString const &equation);
    bool loadBoundaryConditions(QString const &boundary_conditions);
    bool loadParameters(QString const &parameters);
};

#endif // MAINWINDOW_HPP
