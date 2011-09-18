#include "mainframe.hpp"

MainFrame::MainFrame(QWidget *parent)
    : QTabWidget(parent)
{
    // allocate interface
    problem = new QWidget(this);
    problem_layout = new QVBoxLayout(problem);
    equation_tab = new QTabWidget(problem);
    boundary_conditions_tab = new QTabWidget(problem);
    parameters_tab = new QTabWidget(problem);
    geometry_tab = new QTabWidget(problem);
    equation = new QWidget(equation_tab);
    equation_layout = new QHBoxLayout(equation);
    equation_formula = new Formula(equation);
    boundary_conditions = new QWidget(boundary_conditions_tab);
    boundary_conditions_layout = new QHBoxLayout(boundary_conditions);
    border_label = new QLabel(boundary_conditions);
    borders = new QComboBox(boundary_conditions);
    condition_label = new QLabel(boundary_conditions);
    boundary_condition = new Formula(boundary_conditions);
    parameters = new QWidget(parameters_tab);
    parameters_layout = new QHBoxLayout(parameters);
    degree_label = new QLabel(parameters);
    degree = new QLabel(parameters);
    tolerance_label = new QLabel(parameters);
    tolerance = new QLabel(parameters);
    penality_label = new QLabel(parameters);
    penality = new QLabel(parameters);
    geometry_viewer = new Viewer2D;
    solution = new QWidget(this);
    solution_layout = new QVBoxLayout(solution);
    solution_viewer = new Viewer3D;

    // setup interface
    this->addTab(problem, "&Problem");
    this->addTab(solution, "&Solution");
    problem->setLayout(problem_layout);
    problem_layout->addWidget(equation_tab,0);
    problem_layout->addWidget(boundary_conditions_tab,0);
    problem_layout->addWidget(parameters_tab,0);
    problem_layout->addWidget(geometry_tab,1);
    equation_tab->addTab(equation,"Equation");
    equation->setLayout(equation_layout);
    equation_layout->addStretch();
    equation_layout->addWidget(equation_formula);
    equation_layout->addStretch();
    boundary_conditions_tab->addTab(boundary_conditions, "Boundary Conditions");
    boundary_conditions->setLayout(boundary_conditions_layout);
    boundary_conditions_layout->addStretch();
    boundary_conditions_layout->addWidget(border_label);
    boundary_conditions_layout->addWidget(borders);
    boundary_conditions_layout->addStretch();;
    boundary_conditions_layout->addWidget(condition_label);
    boundary_conditions_layout->addWidget(boundary_condition);
    boundary_conditions_layout->addStretch();
    parameters_tab->addTab(parameters, "Parameters");
    parameters->setLayout(parameters_layout);
    parameters_layout->addStretch();
    parameters_layout->addWidget(degree_label);
    parameters_layout->addWidget(degree);
    parameters_layout->addStretch();
    parameters_layout->addWidget(tolerance_label);
    parameters_layout->addWidget(tolerance);
    parameters_layout->addStretch();
    parameters_layout->addWidget(penality_label);
    parameters_layout->addWidget(penality);
    parameters_layout->addStretch();
    equation_formula->setText("none");
    border_label->setText("<b>Border</b>");
    condition_label->setText("<b>Condition</b>");
    boundary_condition->setText("none");
    degree_label->setText("<b>Degree</b>");
    degree->setText("none");
    tolerance_label->setText("<b>Tolerance</b>");
    tolerance->setText("none");
    penality_label->setText("<b>Penality</b>");
    penality->setText("none");
    geometry_tab->addTab(geometry_viewer, "Geometry");
    solution->setLayout(solution_layout);
    solution_layout->addWidget(solution_viewer);
    setTabEnabled(1,false);
    borders->setEnabled(false);
};

MainFrame::~MainFrame()
{
    // free interface
    delete equation;
    delete border_label;
    delete borders;
    delete condition_label;
    delete boundary_condition;
    delete boundary_conditions_layout;
    delete degree_label;
    delete degree;
    delete tolerance_label;
    delete tolerance;
    delete penality_label;
    delete penality;
    delete parameters_layout;
    delete boundary_conditions;
    delete parameters;
    delete geometry_viewer;
    delete equation_tab;
    delete boundary_conditions_tab;
    delete parameters_tab;
    delete geometry_tab;
    delete problem_layout;
    delete problem;
    delete solution_viewer;
    delete solution_layout;
    delete solution;
};

void MainFrame::setBoundaryConditions(const QStringList &labels,
                                      const QStringList &mmls)
{
    bcs_mmls = mmls;
    borders->clear();
    borders->addItems(labels);
    if(borders->count())
    {
        borders->setEnabled(true);
        borders->setCurrentIndex(0);
        displayBoundaryCondition(0);
    }
};

void MainFrame::displayBoundaryCondition(const int &index)
{
    if(index>=0 && index < bcs_mmls.size())
    {
        boundary_condition->setContent(bcs_mmls[index]);
        boundary_condition->setFixedSize(boundary_condition->sizeHint());
    }
};

void MainFrame::resetBoundaryConditions()
{
    bcs_mmls.clear();
    boundary_condition->setText("none");
    boundary_condition->setFixedSize(boundary_condition->sizeHint());
    borders->clear();
    borders->setEnabled(false);
};

