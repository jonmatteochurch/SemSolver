#ifndef MENUBAR_HPP
#define MENUBAR_HPP

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

class MenuBar
    : public QMenuBar
{
    // interface

    QMenu   *file;
    QMenu   *geometry;
    QMenu   *equation;
    QMenu   *boundary_conditions;
    QMenu   *parameters;
    QMenu   *solution;
    QMenu   *help;

public:

    QAction *new_workspace;
    QAction *open_workspace;
    QAction *quit;

    QAction *new_geometry;
    QAction *new_geometry_from_pslg;
    QAction *import_geometry;
    QAction *import_geometry_from_workspace;
    QAction *export_geometry;
    QAction *remove_geometry;

    QAction *new_equation;
    QAction *import_equation;
    QAction *import_equation_from_workspace;
    QAction *export_equation;
    QAction *remove_equation;

    QAction *new_boundary_conditions;
    QAction *import_boundary_conditions;
    QAction *import_boundary_conditions_from_workspace;
    QAction *export_boundary_conditions;
    QAction *remove_boundary_conditions;

    QAction *new_parameters;
    QAction *import_parameters;
    QAction *import_parameters_from_workspace;
    QAction *export_parameters;
    QAction *remove_parameters;

    QAction *lu_solve;
    QAction *qr_solve;
    QAction *cholesky_solve;
    QAction *export_solution;
    QAction *change_plot_style;
    QAction *export_plot;

    QAction *browse_help;
    QAction *about;

    // methods

    MenuBar(QWidget *parent = 0);
    ~MenuBar();

    inline void enableGeometry()
		{
    geometry->setEnabled(true);
};
    inline void enableEquation()
		{
    equation->setEnabled(true);
};

    inline void enableBoundaryConditions()
		{
    boundary_conditions->setEnabled(true);
};

    inline void enableParameters()
		{
    parameters->setEnabled(true);
};

    inline void enableSolution()
		{
    solution->setEnabled(true);
};


    inline void disableGeometry()
		{
    geometry->setEnabled(false);
};

    inline void disableEquation()
		{
    equation->setEnabled(false);
};

    inline void disableBoundaryConditions()
		{
    boundary_conditions->setEnabled(false);
};

    inline void disableParameters()
		{
    parameters->setEnabled(false);
};

    inline void disableSolution()
		{
    export_solution->setEnabled(false);
    change_plot_style->setEnabled(false);
    export_plot->setEnabled(false);
    solution->setEnabled(false);
};
};

#endif // MENUBAR_HPP
