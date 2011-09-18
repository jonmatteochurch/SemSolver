#include "dock.hpp"

Dock::Dock(QWidget *parent)
    : QDockWidget(parent)
{
    // allocate interface

    widget = new QWidget(this);
    layout = new QVBoxLayout(widget);
    workspace = new QGroupBox(widget);
    workspace_layout = new QVBoxLayout(workspace);
    workspace_label = new QLabel(workspace);
    geometry = new QGroupBox(widget);
    geometry_layout = new QVBoxLayout(geometry);
    geometry_list = new QComboBox(geometry);
    equation = new QGroupBox(widget);
    equation_layout = new QVBoxLayout(equation);
    equation_list = new QComboBox(equation);
    boundary_conditions = new QGroupBox(widget);
    boundary_conditions_layout = new QVBoxLayout(boundary_conditions);
    boundary_conditions_list = new QComboBox(boundary_conditions);
    parameters = new QGroupBox(widget);
    parameters_layout = new QVBoxLayout(parameters);
    parameters_list = new QComboBox(parameters);

    // setup interface

    this->setWidget(widget);
    this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    this->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);

    widget->setLayout(layout);

    layout->addWidget(workspace);
    layout->addWidget(geometry);
    layout->addWidget(equation);
    layout->addWidget(boundary_conditions);
    layout->addWidget(parameters);
    layout->addStretch();

    workspace->setTitle("Workspace");
    workspace->setLayout(workspace_layout);
    workspace_layout->addWidget(workspace_label);
    workspace_label->setText("<span style=\" font-style:italic;\">none</span>");

    geometry->setTitle("Geometry");
    geometry->setLayout(geometry_layout);
    geometry_layout->addWidget(geometry_list);
    geometry->setEnabled(false);

    equation->setTitle("Equation");
    equation->setLayout(equation_layout);
    equation_layout->addWidget(equation_list);
    equation->setEnabled(false);

    boundary_conditions->setTitle("Boundary Conditions");
    boundary_conditions->setLayout(boundary_conditions_layout);
    boundary_conditions_layout->addWidget(boundary_conditions_list);
    boundary_conditions->setEnabled(false);

    parameters->setTitle("Parameters");
    parameters->setLayout(parameters_layout);
    parameters_layout->addWidget(parameters_list);
    parameters->setEnabled(false);

    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(refresh(bool)));
};

Dock::~Dock()
{
    // free interface
    delete workspace_layout;
    delete workspace_label;
    delete geometry_layout;
    delete geometry_list;
    delete equation_layout;
    delete equation_list;
    delete boundary_conditions_layout;
    delete boundary_conditions_list;
    delete parameters_layout;
    delete parameters_list;
    delete workspace;
    delete geometry;
    delete equation;
    delete boundary_conditions;
    delete parameters;
    delete layout;
    delete widget;
};

void Dock::refresh(bool is_floating)
{
    if(is_floating)
    {
        this->setWindowTitle("SemSolver-GUI Panel");
        this->adjustSize();
    }
    else
        this->setWindowTitle("");
};

void Dock::setWorkspace(const QFileInfo &workspace)
{
    workspace_label->setText(workspace.baseName());
    workspace_label->setToolTip(workspace.canonicalFilePath());
    geometry->setEnabled(true);
    equation->setEnabled(true);
    boundary_conditions->setEnabled(true);
    parameters->setEnabled(true);
};

void Dock::setGeometries(const QStringList &geometries)
{
    geometry_list->clear();
    geometry_list->addItems(geometries);
};

void Dock::setEquations(const QStringList &equations)
{
    equation_list->clear();
    equation_list->addItems(equations);
};

void Dock::setBoundaryConditions(const QStringList &boundary_conditions)
{
    boundary_conditions_list->clear();
    boundary_conditions_list->addItems(boundary_conditions);
};

void Dock::setParameters(const QStringList &parameters)
{
    parameters_list->clear();
    parameters_list->addItems(parameters);
};

void Dock::resetWorkspace()
{
    workspace_label->setText("");
    workspace_label->setToolTip("");
    geometry->setEnabled(false);
    equation->setEnabled(false);
    boundary_conditions->setEnabled(false);
    parameters->setEnabled(false);
};

void Dock::selectGeometry(const QString &geometry)
{
    int index=0;
    geometry_list->setCurrentIndex(index);
    while(geometry_list->currentText()!=geometry)
        geometry_list->setCurrentIndex(++index);
};

void Dock::selectEquation(const QString &equation)
{
    int index=0;
    equation_list->setCurrentIndex(index);
    while(equation_list->currentText()!=equation)
        equation_list->setCurrentIndex(++index);
};

void Dock::selectBoundaryConditions(const QString &boundary_conditions)
{
    int index=0;
    boundary_conditions_list->setCurrentIndex(index);
    while(boundary_conditions_list->currentText()!=boundary_conditions)
        boundary_conditions_list->setCurrentIndex(++index);
};

void Dock::selectParameters(const QString &parameters)
{
    int index=0;
    parameters_list->setCurrentIndex(index);
    while(parameters_list->currentText()!=parameters)
        parameters_list->setCurrentIndex(++index);
};
