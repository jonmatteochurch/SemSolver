#include "removeboundaryconditionsdialog.hpp"

RemoveBoundaryConditionsDialog::RemoveBoundaryConditionsDialog(
        QWidget *parent,
        const QStringList &existing_boundary_conditions,
        QFile *const workspace)
            : QDialog(parent), boundary_conditions(existing_boundary_conditions), semsol_file(workspace)
{
    layout = new QGridLayout;
    label_boundary_conditions = new QLabel;
    boundary_conditions_list = new QComboBox;
    message = new QLabel;
    cancel = new QPushButton;
    button_remove = new QPushButton;
    label_boundary_conditions->setText("Boundary Conditions:");
    boundary_conditions_list->addItems(boundary_conditions);
    cancel->setText("&Cancel");
    button_remove->setText("&Remove");
    layout->addWidget(label_boundary_conditions,0,0);
    layout->addWidget(boundary_conditions_list,0,1,1,4);
    layout->addWidget(message,1,0,1,5);
    layout->addWidget(cancel,2,3);
    layout->addWidget(button_remove,2,4);
    this->setLayout(layout);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Remove Boundary Conditions");

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_remove, SIGNAL(clicked()), this, SLOT(remove()));
};

RemoveBoundaryConditionsDialog::~RemoveBoundaryConditionsDialog()
{
    delete layout;
    delete label_boundary_conditions;
    delete boundary_conditions_list;
    delete cancel;
    delete button_remove;
};
