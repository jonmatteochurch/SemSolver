#include "removeequationdialog.hpp"

RemoveEquationDialog::RemoveEquationDialog(
        QWidget *parent,
        const QStringList &existing_equations,
        QFile *const workspace)
            : QDialog(parent), equations(existing_equations), semsol_file(workspace)
{
    layout = new QGridLayout;
    label_equation = new QLabel;
    equations_list = new QComboBox;
    message = new QLabel;
    cancel = new QPushButton;
    button_remove = new QPushButton;
    label_equation->setText("Equation:");
    equations_list->addItems(equations);
    cancel->setText("&Cancel");
    button_remove->setText("&Remove");
    layout->addWidget(label_equation,0,0);
    layout->addWidget(equations_list,0,1,1,4);
    layout->addWidget(message,1,0,1,5);
    layout->addWidget(cancel,2,3);
    layout->addWidget(button_remove,2,4);
    this->setLayout(layout);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Remove Equation");

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_remove, SIGNAL(clicked()), this, SLOT(remove()));
};

RemoveEquationDialog::~RemoveEquationDialog()
{
    delete layout;
    delete label_equation;
    delete equations_list;
    delete cancel;
    delete button_remove;
};
