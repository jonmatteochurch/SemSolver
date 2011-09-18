#include "mainwindow.hpp"

#include <QMessageBox>

#include "../lib/semsolver/matrix.hpp"
#include "../lib/semsolver/vector.hpp"
#include "../lib/semsolver/semgeometry.hpp"
#include "../lib/semsolver/equation.hpp"
#include "../lib/semsolver/semparameters.hpp"
#include "../lib/semsolver/semspace.hpp"
#include "../lib/semsolver-io/workspace.hpp"
#include "../lib/semsolver-assembler/computealgebraicsystem.hpp"
#include "../lib/semsolver-solver/choleskysolve.hpp"
#include "../lib/semsolver-solver/lusolve.hpp"
#include "../lib/semsolver-solver/qrsolve.hpp"
#include "../lib/semsolver-postprocessor/buildsolution.hpp"
#include "../lib/semsolver-postprocessor/computesolutionhull.hpp"
#include "../lib/semsolver-postprocessor/computeplotdata.hpp"

#include "newworkspacedialog.hpp"
#include "openworkspacedialog.hpp"
#include "newgeometrydialog.hpp"
#include "newgeometryfrompslgdialog.hpp"
#include "importgeometrydialog.hpp"
#include "importgeometryfromworkspacedialog.hpp"
#include "exportgeometrydialog.hpp"
#include "removegeometrydialog.hpp"
#include "newequationdialog.hpp"
#include "importequationdialog.hpp"
#include "importequationfromworkspacedialog.hpp"
#include "exportequationdialog.hpp"
#include "removeequationdialog.hpp"
#include "newboundaryconditionsdialog.hpp"
#include "importboundaryconditionsdialog.hpp"
#include "importboundaryconditionsfromworkspacedialog.hpp"
#include "exportboundaryconditionsdialog.hpp"
#include "removeboundaryconditionsdialog.hpp"
#include "newparametersdialog.hpp"
#include "importparametersdialog.hpp"
#include "importparametersfromworkspacedialog.hpp"
#include "exportparametersdialog.hpp"
#include "removeparametersdialog.hpp"
#include "exportsolutiondialog.hpp"
#include "exportplotdialog.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // allocate interface
    main_frame = new MainFrame(this);
    dock = new Dock(this);
    menu_bar = new MenuBar(this);
    status_bar = new QStatusBar(this);

    // empty files
    workspace = 0;
    geometry = 0;
    equation = 0;
    boundary_conditions = 0;
    parameters = 0;

    // variables
    problem = new SemSolver::Problem<2,double>();
    space = 0;
    solution_function = 0;
    plot_style = 0;

    // setup interface
    this->setCentralWidget(main_frame);
    this->setMenuBar(menu_bar);
    this->setStatusBar(status_bar);
    this->addDockWidget(Qt::RightDockWidgetArea, dock);
    this->resize(600,400);

    // setup connections
    connect(menu_bar->new_workspace, SIGNAL(triggered()), this, SLOT(newWorkspace()));
    connect(menu_bar->open_workspace, SIGNAL(triggered()), this, SLOT(openWorkspace()));
    connect(menu_bar->quit, SIGNAL(triggered()), this, SLOT(close()));

    connect(menu_bar->new_geometry, SIGNAL(triggered()), this, SLOT(newGeometry()));
    connect(menu_bar->new_geometry_from_pslg, SIGNAL(triggered()), this,
            SLOT(newGeometryFromPslg()));
    connect(menu_bar->import_geometry, SIGNAL(triggered()), this, SLOT(importGeometry()));
    connect(menu_bar->import_geometry_from_workspace, SIGNAL(triggered()), this,
            SLOT(importGeometryFromWorkspace()));
    connect(menu_bar->export_geometry, SIGNAL(triggered()), this, SLOT(exportGeometry()));
    connect(menu_bar->remove_geometry, SIGNAL(triggered()), this, SLOT(removeGeometry()));

    connect(menu_bar->new_equation, SIGNAL(triggered()), this, SLOT(newEquation()));
    connect(menu_bar->import_equation, SIGNAL(triggered()), this, SLOT(importEquation()));
    connect(menu_bar->import_equation_from_workspace, SIGNAL(triggered()), this,
            SLOT(importEquationFromWorkspace()));
    connect(menu_bar->export_equation, SIGNAL(triggered()), this, SLOT(exportEquation()));
    connect(menu_bar->remove_equation, SIGNAL(triggered()), this, SLOT(removeEquation()));

    connect(menu_bar->new_boundary_conditions, SIGNAL(triggered()), this, SLOT(newBoundaryConditions()));
    connect(menu_bar->import_boundary_conditions, SIGNAL(triggered()), this, SLOT(importBoundaryConditions()));
    connect(menu_bar->import_boundary_conditions_from_workspace, SIGNAL(triggered()), this,
            SLOT(importBoundaryConditionsFromWorkspace()));
    connect(menu_bar->export_boundary_conditions, SIGNAL(triggered()), this, SLOT(exportBoundaryConditions()));
    connect(menu_bar->remove_boundary_conditions, SIGNAL(triggered()), this, SLOT(removeBoundaryConditions()));

    connect(menu_bar->new_parameters, SIGNAL(triggered()), this, SLOT(newParameters()));
    connect(menu_bar->import_parameters, SIGNAL(triggered()), this, SLOT(importParameters()));
    connect(menu_bar->import_parameters_from_workspace, SIGNAL(triggered()), this,
            SLOT(importParametersFromWorkspace()));
    connect(menu_bar->export_parameters, SIGNAL(triggered()), this, SLOT(exportParameters()));
    connect(menu_bar->remove_parameters, SIGNAL(triggered()), this, SLOT(removeParameters()));

    connect(menu_bar->lu_solve, SIGNAL(triggered()), this, SLOT(solveLU()));
    connect(menu_bar->qr_solve, SIGNAL(triggered()), this, SLOT(solveQR()));
    connect(menu_bar->cholesky_solve, SIGNAL(triggered()), this, SLOT(solveCholesky()));
    connect(menu_bar->export_solution, SIGNAL(triggered()), this, SLOT(exportSolution()));
    connect(menu_bar->change_plot_style, SIGNAL(triggered()), this, SLOT(changePlotStyle()));
    connect(menu_bar->export_plot, SIGNAL(triggered()), this, SLOT(exportPlot()));

    connect(main_frame->borders, SIGNAL(activated(int)), this,
            SLOT(displayBoundaryCondition(int)));

    connect(dock->geometry_list, SIGNAL(activated(const QString &)), this,
            SLOT(loadGeometry(QString)));
    connect(dock->equation_list, SIGNAL(activated(const QString &)), this,
            SLOT(loadEquation(QString)));
    connect(dock->boundary_conditions_list, SIGNAL(activated(const QString &)), this,
            SLOT(loadBoundaryConditions(QString)));
    connect(dock->parameters_list, SIGNAL(activated(const QString &)), this,
            SLOT(loadParameters(QString)));
};

MainWindow::~MainWindow()
{
    // free interface
    delete main_frame;
    delete dock;
    delete menu_bar;
    delete status_bar;

    // free files
    delete workspace;
    delete geometry;
    delete equation;
    delete boundary_conditions;
    delete parameters;

    // free variable
    delete problem;
    delete space;
    delete solution_function;
};

void MainWindow::newWorkspace()
{
    NewWorkspaceDialog dialog(this);
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error creating new Workspace file.");
    if(!dialog.exec())
        return;
    QFile file(dialog.fileName());
    if(file.exists())
    {
        if(!file.remove())
        {
            message.exec();
            return;
        }
    }
    if(!file.open(QIODevice::WriteOnly))
    {
        message.exec();
        return;
    }
    file.close();
    if(!loadWorkspace(file.fileName()))
    {
        message.exec();
        resetWorkspace();
    }
};

void MainWindow::openWorkspace()
{
    OpenWorkspaceDialog dialog(this);
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error opening Workspace file.");
    if(!dialog.exec())
        return;
    if(!loadWorkspace(dialog.fileName()))
    {
        message.exec();
        resetWorkspace();
    }
};

void MainWindow::newGeometry()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error creating new Geometry.");
    QStringList geometries;
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    NewGeometryDialog dialog(this, geometries);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name()+".semgeo", dialog.file());
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    resetGeometry();

    dock->setGeometries(geometries);

    if(!loadGeometry(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::newGeometryFromPslg()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error creating new Geometry from PSLG.");
    QStringList geometries;
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    NewGeometryFromPslgDialog dialog(this, geometries);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name()+".semgeo", dialog.file());
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    resetGeometry();

    dock->setGeometries(geometries);

    if(!loadGeometry(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::importGeometry()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error importing Geometry.");
    QStringList geometries;
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    ImportGeometryDialog dialog(this, geometries);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name() + ".semgeo", dialog.file());
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    resetGeometry();

    dock->setGeometries(geometries);
    if(!loadGeometry(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::importGeometryFromWorkspace()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error importing Geometry from workspace.");
    QStringList geometries;
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    ImportGeometryFromWorkspaceDialog dialog(this, geometries);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name() + ".semgeo", dialog.file());
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    resetGeometry();

    dock->setGeometries(geometries);

    if(!loadGeometry(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::exportGeometry()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error exporting Geometry from workspace.");
    QStringList geometries;
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    ExportGeometryDialog dialog(this, geometries, workspace);
    dialog.exec();
};

void MainWindow::removeGeometry()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error removing Geometry from workspace.");
    QStringList geometries;
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }
    RemoveGeometryDialog dialog(this, geometries, workspace);
    if(!dialog.exec())
        return;
    QString current_geometry(dock->geometry_list->currentText());
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
    {
        message.exec();
        return;
    }

    dock->setGeometries(geometries);
    if(geometries.contains(current_geometry))
        dock->selectGeometry(current_geometry);
    else
    {
        resetGeometry();
        if(!geometries.isEmpty())
        {
            if(!loadGeometry(geometries[0]))
            {
                message.exec();
                return;
            }
        }
    }
};

void MainWindow::newEquation()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error creating new Equation.");
    QStringList equations;
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
    {
        message.exec();
        return;
    }
    NewEquationDialog dialog(this, equations);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name()+".semeqn", dialog.file());
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
    {
        message.exec();
        return;
    }
    resetEquation();

    dock->setEquations(equations);

    if(!loadEquation(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::importEquation()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error importing Equation.");
    QStringList equations;
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
    {
        message.exec();
        return;
    }
    ImportEquationDialog dialog(this, equations);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name() + ".semeqn", dialog.file());
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
    {
        message.exec();
        return;
    }
    resetEquation();

    dock->setEquations(equations);
    if(!loadEquation(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::importEquationFromWorkspace()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error importing Equation from workspace.");
    QStringList equations;
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
    {
        message.exec();
        return;
    }
    ImportEquationFromWorkspaceDialog dialog(this, equations);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name() + ".semeqn", dialog.file());
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
    {
        message.exec();
        return;
    }
    resetEquation();

    dock->setEquations(equations);

    if(!loadEquation(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::exportEquation()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error exporting Equation from workspace.");
    QStringList equations;
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
    {
        message.exec();
        return;
    }
    ExportEquationDialog dialog(this, equations, workspace);
    dialog.exec();
};

void MainWindow::removeEquation()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error removing Equation from workspace.");
    QStringList equations;
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
    {
        message.exec();
        return;
    }
    RemoveEquationDialog dialog(this, equations, workspace);
    if(!dialog.exec())
        return;
    QString current_equation(dock->equation_list->currentText());
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
    {
        message.exec();
        return;
    }

    dock->setEquations(equations);
    if(equations.contains(current_equation))
        dock->selectEquation(current_equation);
    else
    {
        resetEquation();
        if(!equations.isEmpty())
        {
            if(!loadEquation(equations[0]))
            {
                message.exec();
                return;
            }
        }
    }
};

void MainWindow::newBoundaryConditions()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error creating new Boundary Conditions.");
    QStringList boundary_conditions;
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions))
    {
        message.exec();
        return;
    }
    NewBoundaryConditionsDialog dialog(this, boundary_conditions);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name()+".sembcs", dialog.file());
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions))
    {
        message.exec();
        return;
    }
    resetBoundaryConditions();

    dock->setBoundaryConditions(boundary_conditions);

    if(!loadBoundaryConditions(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::importBoundaryConditions()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error importing Boundary Conditions.");
    QStringList boundary_conditions;
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions))
    {
        message.exec();
        return;
    }
    ImportBoundaryConditionsDialog dialog(this, boundary_conditions);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name() + ".sembcs", dialog.file());
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions))
    {
        message.exec();
        return;
    }
    resetBoundaryConditions();

    dock->setBoundaryConditions(boundary_conditions);
    if(!loadBoundaryConditions(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::importBoundaryConditionsFromWorkspace()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error importing Boundary Conditions from workspace.");
    QStringList boundary_conditions;
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions))
    {
        message.exec();
        return;
    }
    ImportBoundaryConditionsFromWorkspaceDialog dialog(this, boundary_conditions);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name() + ".sembcs", dialog.file());
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions))
    {
        message.exec();
        return;
    }
    resetBoundaryConditions();

    dock->setBoundaryConditions(boundary_conditions);

    if(!loadBoundaryConditions(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::exportBoundaryConditions()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error exporting Boundary Conditions from workspace.");
    QStringList boundary_conditions;
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions))
    {
        message.exec();
        return;
    }
    ExportBoundaryConditionsDialog dialog(this, boundary_conditions, workspace);
    dialog.exec();
};

void MainWindow::removeBoundaryConditions()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error removing Boundary Conditions from workspace.");
    QStringList boundary_conditions;
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions))
    {
        message.exec();
        return;
    }
    RemoveBoundaryConditionsDialog dialog(this, boundary_conditions, workspace);
    if(!dialog.exec())
        return;
    QString current_boundary_conditions(dock->boundary_conditions_list->currentText());
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions))
    {
        message.exec();
        return;
    }

    dock->setBoundaryConditions(boundary_conditions);
    if(boundary_conditions.contains(current_boundary_conditions))
        dock->selectBoundaryConditions(current_boundary_conditions);
    else
    {
        resetBoundaryConditions();
        if(!boundary_conditions.isEmpty())
        {
            if(!loadBoundaryConditions(boundary_conditions[0]))
            {
                message.exec();
                return;
            }
        }
    }
};

void MainWindow::newParameters()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error creating new Parameters.");
    QStringList parameters;
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
    {
        message.exec();
        return;
    }
    NewParametersDialog dialog(this, parameters);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name()+".semprm", dialog.file());
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
    {
        message.exec();
        return;
    }
    resetParameters();

    dock->setParameters(parameters);

    if(!loadParameters(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::importParameters()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error importing Parameters.");
    QStringList parameters;
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
    {
        message.exec();
        return;
    }
    ImportParametersDialog dialog(this, parameters);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name() + ".semprm", dialog.file());
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
    {
        message.exec();
        return;
    }
    resetParameters();

    dock->setParameters(parameters);
    if(!loadParameters(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::importParametersFromWorkspace()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error importing Parameters from workspace.");
    QStringList parameters;
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
    {
        message.exec();
        return;
    }
    ImportParametersFromWorkspaceDialog dialog(this, parameters);
    if(!dialog.exec())
        return;
    SemSolver::IO::add_file_to_workspace(workspace, dialog.name() + ".semprm", dialog.file());
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
    {
        message.exec();
        return;
    }
    resetParameters();

    dock->setParameters(parameters);

    if(!loadParameters(dialog.name()))
    {
        message.exec();
        return;
    }
};

void MainWindow::exportParameters()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error exporting Parameters from workspace.");
    QStringList parameters;
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
    {
        message.exec();
        return;
    }
    ExportParametersDialog dialog(this, parameters, workspace);
    dialog.exec();
};

void MainWindow::removeParameters()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error removing Parameters from workspace.");
    QStringList parameters;
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
    {
        message.exec();
        return;
    }
    RemoveParametersDialog dialog(this, parameters, workspace);
    if(!dialog.exec())
        return;
    QString current_parameters(dock->parameters_list->currentText());
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
    {
        message.exec();
        return;
    }

    dock->setParameters(parameters);
    if(parameters.contains(current_parameters))
        dock->selectParameters(current_parameters);
    else
    {
        resetParameters();
        if(!parameters.isEmpty())
        {
            if(!loadParameters(parameters[0]))
            {
                message.exec();
                return;
            }
        }
    }
};

bool MainWindow::loadWorkspace(QString const &file)
{
    delete workspace;
    workspace = new QFile(file);
    QStringList geometries, equations, boundary_conditions, parameters;
    if(!SemSolver::IO::get_geometries_list_from_workspace(workspace, geometries))
        return false;
    if(!SemSolver::IO::get_equations_list_from_workspace(workspace, equations))
        return false;
    if(!SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace,
                                                                   boundary_conditions))
        return false;
    if(!SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters))
        return false;

    resetGeometry();
    resetEquation();
    resetBoundaryConditions();
    resetParameters();
    resetSolution();

    dock->setWorkspace(*workspace);
    dock->setGeometries(geometries);
    dock->setEquations(equations);
    dock->setBoundaryConditions(boundary_conditions);
    dock->setParameters(parameters);

    menu_bar->enableGeometry();
    menu_bar->enableEquation();
    menu_bar->enableBoundaryConditions();
    menu_bar->enableParameters();

    if(!geometries.isEmpty())
        if(!loadGeometry(geometries[0]))
            return false;

    if(!equations.isEmpty())
        if(!loadEquation(equations[0]))
            return false;

    if(!boundary_conditions.isEmpty())
        if(!loadBoundaryConditions(boundary_conditions[0]))
            return false;

    if(!parameters.isEmpty())
        if(!loadParameters(parameters[0]))
            return false;

    return true;
};

bool MainWindow::loadGeometry(const QString &name)
{
    SemSolver::SemGeometry<2,double> geometry;
    SemSolver::IO::get_geometry_from_workspace(workspace, name, geometry);
    dock->selectGeometry(name);
    main_frame->plotGeometry(geometry);
    problem->setGeometry(new SemSolver::SemGeometry<2,double>(geometry));
    resetSolution();
    if(problem->isDefined())
        menu_bar->enableSolution();
    return true;
};

bool MainWindow::loadEquation(const QString &name)
{
    SemSolver::Equation<2, double> *equation = 0;
    if(!SemSolver::IO::get_equation_from_workspace(workspace, name, equation))
        return false;
    dock->selectEquation(name);
    main_frame->displayEquation(equation->mml());

    problem->setEquation(equation);
    resetSolution();
    if(problem->isDefined())
        menu_bar->enableSolution();
    return true;
};

bool MainWindow::loadBoundaryConditions(const QString &name)
{
    SemSolver::BoundaryConditions<2, double> boundary_conditions;
    SemSolver::IO::get_boundary_conditions_from_workspace(workspace, name,
                                                          boundary_conditions);
    dock->selectBoundaryConditions(name);
    main_frame->setBoundaryConditions(boundary_conditions.labels(),
                                      boundary_conditions.mmls());
    problem->setBoundaryConditions(
            new SemSolver::BoundaryConditions<2, double>(boundary_conditions));
    resetSolution();
    if(problem->isDefined())
        menu_bar->enableSolution();
    return true;
};

bool MainWindow::loadParameters(const QString &name)
{
    SemSolver::SemParameters<double> parameters;
    SemSolver::IO::get_parameters_from_workspace(workspace, name, parameters);
    dock->selectParameters(name);
    main_frame->displayParameters(parameters);
    problem->setParameters(new SemSolver::SemParameters<double>(parameters));
    resetSolution();
    if(problem->isDefined())
        menu_bar->enableSolution();
    return true;
};

void MainWindow::resetWorkspace()
{
    delete workspace;
    workspace = 0;

    dock->resetWorkspace();

    menu_bar->disableGeometry();
    menu_bar->disableEquation();
    menu_bar->disableBoundaryConditions();
    menu_bar->disableParameters();
    menu_bar->disableSolution();

    resetGeometry();
    resetEquation();
    resetBoundaryConditions();
    resetParameters();
    resetSolution();
};

void MainWindow::resetGeometry()
{
    problem->clearGeometry();
    main_frame->resetGeometry();
    resetSolution();
};

void MainWindow::resetEquation()
{
    problem->clearEquation();
    main_frame->resetEquation();
    resetSolution();
};

void MainWindow::resetBoundaryConditions()
{
    problem->clearBoundaryConditions();
    main_frame->resetBoundaryConditions();
    resetSolution();
};

void MainWindow::resetParameters()
{
    problem->clearParameters();
    main_frame->resetParameters();
    resetSolution();
};

void MainWindow::resetSolution()
{
    main_frame->resetSolution();
    menu_bar->disableSolution();
    delete space;
    delete solution_function;
    solution_data.clear();
    solution_poly.clear();
    space = 0;
    solution_function = 0;
};

void MainWindow::solveLU()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Problem is singular.");

    qDebug() << "PREPROCESSING";
    status_bar->showMessage("Pre-processing...");
    space = new SemSolver::SemSpace<2, double>(*problem->geometry(), *problem->parameters());
    qDebug() << "ASSEMBLING";
    status_bar->showMessage("Assembling...");
    SemSolver::Assembler::compute_algebraic_system(*space, *problem, problem_matrix, problem_vector);
    qDebug() << "SOLVING";
    status_bar->showMessage("Solving...");
    if(!SemSolver::Solver::lu_solve(problem_matrix, problem_vector, solution_vector))
    {
        message.exec();
        return;
    };
    qDebug() << "POSTPROCESSING";
    status_bar->showMessage("Post-processing...");
    SemSolver::PostProcessor::compute_plot_data(*space, solution_vector, solution_data, solution_poly);
    SemSolver::PostProcessor::build_solution(*space, solution_vector, solution_function);
    SemSolver::PostProcessor::compute_solution_hull(*space, solution_vector, xmin, ymin, zmin, xmax, ymax, zmax);
    qDebug() << "DONE";
    status_bar->showMessage("Done!");
    plotSolution();
    main_frame->setCurrentIndex(1);
    menu_bar->export_solution->setEnabled(true);
    menu_bar->change_plot_style->setEnabled(true);
    menu_bar->export_plot->setEnabled(true);
    return;
};

void MainWindow::solveQR()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Problem is not full rank.");

    qDebug() << "PREPROCESSING";
    status_bar->showMessage("Pre-processing...");
    space = new SemSolver::SemSpace<2, double>(*problem->geometry(), *problem->parameters());
    qDebug() << "ASSEMBLING";
    status_bar->showMessage("Assembling...");
    SemSolver::Assembler::compute_algebraic_system(*space, *problem, problem_matrix, problem_vector);
    qDebug() << "SOLVING";
    status_bar->showMessage("Solving...");
    if(!SemSolver::Solver::qr_solve(problem_matrix, problem_vector, solution_vector))
    {
        message.exec();
        return;
    };
    qDebug() << "POSTPROCESSING";
    status_bar->showMessage("Post-processing...");
    SemSolver::PostProcessor::compute_plot_data(*space, solution_vector, solution_data, solution_poly);
    SemSolver::PostProcessor::build_solution(*space, solution_vector, solution_function);
    SemSolver::PostProcessor::compute_solution_hull(*space, solution_vector, xmin, ymin, zmin, xmax, ymax, zmax);
    qDebug() << "DONE";
    status_bar->showMessage("Done!");
    plotSolution();
    main_frame->setCurrentIndex(1);
    menu_bar->export_solution->setEnabled(true);
    menu_bar->change_plot_style->setEnabled(true);
    menu_bar->export_plot->setEnabled(true);
    return;
};

void MainWindow::solveCholesky()
{
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Problem is not symmetric, positive definite.");

    qDebug() << "PREPROCESSING";
    status_bar->showMessage("Pre-processing...");
    space = new SemSolver::SemSpace<2, double>(*problem->geometry(), *problem->parameters());
    qDebug() << "ASSEMBLING";
    status_bar->showMessage("Assembling...");
    SemSolver::Assembler::compute_algebraic_system(*space, *problem, problem_matrix, problem_vector);
    qDebug() << "SOLVING";
    status_bar->showMessage("Solving...");
    if(!SemSolver::Solver::cholesky_solve(problem_matrix, problem_vector, solution_vector))
    {
        message.exec();
        return;
    };
    qDebug() << "POSTPROCESSING";
    status_bar->showMessage("Post-processing...");
    SemSolver::PostProcessor::compute_plot_data(*space, solution_vector, solution_data, solution_poly);
    SemSolver::PostProcessor::build_solution(*space, solution_vector, solution_function);
    SemSolver::PostProcessor::compute_solution_hull(*space, solution_vector, xmin, ymin, zmin, xmax, ymax, zmax);
    qDebug() << "DONE";
    status_bar->showMessage("Done!");
    plotSolution();
    main_frame->setCurrentIndex(1);
    menu_bar->export_solution->setEnabled(true);
    menu_bar->change_plot_style->setEnabled(true);
    menu_bar->export_plot->setEnabled(true);
    return;
};

void MainWindow::exportSolution()
{
    ExportSolutionDialog dialog(this);
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error creating solution file.");
    if(!dialog.exec())
        return;
    QFile file(dialog.fileName());
    if(file.exists())
    {
        if(!file.remove())
        {
            message.exec();
            return;
        }
    }
    if(!file.open(QIODevice::WriteOnly))
    {
        message.exec();
        return;
    }
    QTextStream out(&file);
    int n = solution_vector.rows();
    for(int i=0; i<n; ++i)
        out << space->node(i).point().x() << "\t" << space->node(i).point().y() << "\t" << solution_vector[i] << "\n" ;
    file.close();
};

void MainWindow::changePlotStyle()
{
    if(plot_style == 0)
    {
        plot_style = 1;
        menu_bar->change_plot_style->setText("&View solution nodes");
    }
    else
    {
        plot_style = 0;
        menu_bar->change_plot_style->setText("&View solution surface");
    }
    plotSolution();
}

void MainWindow::exportPlot()
{
    ExportPlotDialog dialog(this);
    QMessageBox message(this);
    message.setWindowTitle("Error");
    message.setText("Error creating plot file.");
    if(!dialog.exec())
        return;
    QFile file(dialog.fileName());
    if(file.exists())
    {
        if(!file.remove())
        {
            message.exec();
            return;
        }
    }
    if(!main_frame->exportSolution(dialog.fileName(), dialog.format()))
    {
        message.exec();
        return;
    }
};

void MainWindow::plotSolution()
{
    if(plot_style == 1)
        main_frame->plotSolution(solution_function, xmin, ymin, xmax, ymax);
    else
        main_frame->plotSolution(solution_data, solution_poly);
}
