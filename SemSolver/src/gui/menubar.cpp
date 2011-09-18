#include "menubar.hpp"

MenuBar::MenuBar(QWidget *parent)
    : QMenuBar(parent)
{
    // allocate interface

    file = new QMenu("&File", this);
    new_workspace = new QAction("&New Workspace...", file);
    open_workspace = new QAction("&Open Workspace...", file);
    quit = new QAction("&Quit",file);

    geometry = new QMenu("&Geometry",this);
    new_geometry = new QAction("&New Geometry...", geometry);
    new_geometry_from_pslg = new QAction("New Geometry from &PSLG...", geometry);
    import_geometry = new QAction("&Import Geometry", geometry);
    import_geometry_from_workspace = new QAction("Import Geometry from &Workspace",
                                                 geometry);
    export_geometry = new QAction("&Export Geometry", geometry);
    remove_geometry = new QAction("&Remove Geometry", geometry);

    equation = new QMenu("&Equation", this);
    new_equation = new QAction("&New Equation...", equation);
    import_equation = new QAction("&Import Equation", equation);
    import_equation_from_workspace = new QAction("Import Equation from &Workspace",
                                                 equation);
    export_equation = new QAction("&Export Equation", equation);
    remove_equation = new QAction("&Remove Equation", equation);

    boundary_conditions = new QMenu("&Boundary Conditions", this);
    new_boundary_conditions = new QAction("&New Boundary Conditions...",
                                          boundary_conditions);
    import_boundary_conditions = new QAction("&Import Boundary Conditions",
                                             boundary_conditions);
    import_boundary_conditions_from_workspace = new QAction("Import Boundary Conditions "\
                                                            "from &Workspace",
                                                            boundary_conditions);
    export_boundary_conditions = new QAction("&Export Boundary Conditions",
                                             boundary_conditions);
    remove_boundary_conditions = new QAction("&Remove Boundary Conditions",
                                             boundary_conditions);

    parameters = new QMenu("&Parameters", this);
    new_parameters = new QAction("&New Parameters...", parameters);
    import_parameters = new QAction("&Import Parameters", parameters);
    import_parameters_from_workspace = new QAction("Import Parameters from &Workspace",
                                                   parameters);
    export_parameters = new QAction("&Export Parameters", parameters);
    remove_parameters = new QAction("&Remove Parameters", parameters);

    solution = new QMenu("&Solution", this);
    lu_solve = new QAction("Solve with &LU decomposition", solution);
    qr_solve = new QAction("Solve with &QR decomposition", solution);
    cholesky_solve = new QAction("Solve with &Cholesky decomposition", solution);
    export_solution = new QAction("&Export Solution", solution);
    change_plot_style = new QAction("&View solution surface", solution);
    export_plot = new QAction("Export &Plot", solution);

    help = new QMenu("&Help", this);
    browse_help = new QAction("Browse &Help", help);
    about = new QAction("&About", help);

    // setup interface

    this->addMenu(file);
    file->addAction(new_workspace);
    file->addAction(open_workspace);
    file->addSeparator();
    file->addAction(quit);
    new_workspace->setShortcut(QKeySequence::New);
    new_workspace->setStatusTip("Create a new workspace");
    open_workspace->setShortcut(QKeySequence::Open);
    open_workspace->setStatusTip("Open an existing workspace");
    quit->setShortcuts(QKeySequence::Quit);
    quit->setStatusTip("Quit the application");

    this->addMenu(geometry);
    geometry->addAction(new_geometry);
    geometry->addAction(new_geometry_from_pslg);
    geometry->addSeparator();
    geometry->addAction(remove_geometry);
    geometry->addSeparator();
    geometry->addAction(import_geometry);
    geometry->addAction(import_geometry_from_workspace);
    geometry->addSeparator();
    geometry->addAction(export_geometry);
    new_geometry->setStatusTip("Create a new geometry");
    new_geometry_from_pslg->setStatusTip("Create a new geometry from a PSLG file");
    import_geometry->setStatusTip("Import an existing geometry into workspace");
    import_geometry_from_workspace->setStatusTip("Import an existing geometry from " \
                                                 "another workspace");
    export_geometry->setStatusTip("Export geometry");
    remove_geometry->setStatusTip("Remove geometry");
    geometry->setEnabled(false);

    this->addMenu(equation);
    equation->addAction(new_equation);
    equation->addSeparator();
    equation->addAction(remove_equation);
    equation->addSeparator();
    equation->addAction(import_equation);
    equation->addAction(import_equation_from_workspace);
    equation->addSeparator();
    equation->addAction(export_equation);
    new_equation->setStatusTip("Create a new equation");
    import_equation->setStatusTip("Import an existing equation into workspace");
    import_equation_from_workspace->setStatusTip("Import an existing equation from " \
                                                  "another workspace");
    export_equation->setStatusTip("Export equation");
    remove_equation->setStatusTip("Remove equation");
    equation->setEnabled(false);

    this->addMenu(boundary_conditions);
    boundary_conditions->addAction(new_boundary_conditions);
    boundary_conditions->addSeparator();
    boundary_conditions->addAction(remove_boundary_conditions);
    boundary_conditions->addSeparator();
    boundary_conditions->addAction(import_boundary_conditions);
    boundary_conditions->addAction(import_boundary_conditions_from_workspace);
    boundary_conditions->addSeparator();
    boundary_conditions->addAction(export_boundary_conditions);
    new_boundary_conditions->setStatusTip("Create new boundary conditions");
    import_boundary_conditions->setStatusTip("Import existing boundary contions into "\
                                             "workspace");
    import_boundary_conditions_from_workspace->setStatusTip("Import existing boundary "\
                                                            "conditions from another "\
                                                            "workspace");
    export_boundary_conditions->setStatusTip("Export boundary conditions");
    remove_boundary_conditions->setStatusTip("Remove boundary conditions");
    boundary_conditions->setEnabled(false);

    this->addMenu(parameters);
    parameters->addAction(new_parameters);
    parameters->addSeparator();
    parameters->addAction(remove_parameters);
    parameters->addSeparator();
    parameters->addAction(import_parameters);
    parameters->addAction(import_parameters_from_workspace);
    parameters->addSeparator();
    parameters->addAction(export_parameters);
    new_parameters->setStatusTip("Create a new parameters");
    import_parameters->setStatusTip("Import an existing parameters into workspace");
    import_parameters_from_workspace->setStatusTip("Import an existing parameters from "\
                                                   "another workspace");
    export_parameters->setStatusTip("Export Parameters");
    remove_parameters->setStatusTip("Remove Parameters");
    parameters->setEnabled(false);

    this->addMenu(solution);
    solution->addAction(lu_solve);
    solution->addAction(qr_solve);
    solution->addAction(cholesky_solve);
    solution->addSeparator();
    solution->addAction(export_solution);
    solution->addSeparator();
    solution->addAction(change_plot_style);
    solution->addAction(export_plot);
    lu_solve->setStatusTip("Compute solution with LU decomposition");
    qr_solve->setStatusTip("Compute solution with QR decomposition");
    cholesky_solve->setStatusTip("Compute solution with Cholesky decomposition");
    export_solution->setStatusTip("Export solution to file");
    change_plot_style->setStatusTip("Change plot style");
    export_plot->setStatusTip("Export plot to file");
    export_solution->setEnabled(false);
    change_plot_style->setEnabled(false);
    export_plot->setEnabled(false);
    solution->setEnabled(false);

    this->addMenu(help);
    help->addAction(browse_help);
    help->addSeparator();
    help->addAction(about);
    browse_help->setShortcuts(QKeySequence::HelpContents);
    browse_help->setStatusTip("Help on SemSolver");
    about->setStatusTip("Show info about SemSolver");;
};

MenuBar::~MenuBar()
{
    // free interface
    delete new_workspace;
    delete open_workspace;
    delete quit;
    delete file;

    delete new_geometry;
    delete new_geometry_from_pslg;
    delete import_geometry;
    delete import_geometry_from_workspace;
    delete export_geometry;
    delete remove_geometry;
    delete geometry;

    delete new_equation;
    delete import_equation;
    delete import_equation_from_workspace;
    delete export_equation;
    delete remove_equation;
    delete equation;

    delete new_boundary_conditions;
    delete import_boundary_conditions;
    delete import_boundary_conditions_from_workspace;
    delete export_boundary_conditions;
    delete remove_boundary_conditions;
    delete boundary_conditions;

    delete new_parameters;
    delete import_parameters;
    delete import_parameters_from_workspace;
    delete export_parameters;
    delete remove_parameters;
    delete parameters;

    delete lu_solve;
    delete qr_solve;
    delete cholesky_solve;
    delete solution;
    delete export_solution;
    delete change_plot_style;
    delete export_plot;

    delete browse_help;
    delete about;
    delete help;
};
