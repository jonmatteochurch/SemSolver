#include "exportboundaryconditionsdialog.hpp"

#include <QFileDialog>

ExportBoundaryConditionsDialog::ExportBoundaryConditionsDialog(
        QWidget *parent,
        QStringList &existing_boundary_conditions,
        QFile *const workspace)
            : QDialog(parent), boundary_conditions(existing_boundary_conditions),
            semsol_file(workspace)
{
    layout = new QGridLayout(this);
    label_boundary_conditions= new QLabel(this);
    boundary_conditions_list = new QComboBox(this);
    message = new QLabel(this);
    label_name = new QLabel(this);
    line_name = new QLineEdit(this);
    button_browse = new QPushButton(this);
    cancel = new QPushButton(this);
    button_export = new QPushButton(this);
    label_boundary_conditions->setText("Boundary Conditions:");
    boundary_conditions_list->addItems(boundary_conditions);
    label_name->setText("Name:");
    button_browse->setText("&Browse");
    cancel->setText("&Cancel");
    button_export->setText("&Export");
    button_export->setEnabled(false);
    layout->addWidget(label_boundary_conditions,0,0);
    layout->addWidget(boundary_conditions_list,0,1,1,4);
    layout->addWidget(message,1,0,1,5);
    layout->addWidget(label_name,2,0);
    layout->addWidget(line_name,2,1);
    layout->addWidget(button_browse,2,2);
    layout->addWidget(cancel,2,3);
    layout->addWidget(button_export,2,4);
    this->setLayout(layout);

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_export, SIGNAL(clicked()), this, SLOT(exportFile()));
    connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));
    connect(button_browse, SIGNAL(clicked()), this, SLOT(browse()));

    sembcs_file = 0;

    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Export Boundary Conditions");
};

ExportBoundaryConditionsDialog::~ExportBoundaryConditionsDialog()
{
    delete layout;
    delete label_boundary_conditions;
    delete boundary_conditions_list;
    delete label_name;
    delete line_name;
    delete button_browse;
    delete cancel;
    delete button_export;
    delete sembcs_file;
}

void ExportBoundaryConditionsDialog::checkName()
{
    QString string_name = line_name->text();
    if(string_name.isEmpty())
    {
        message->setText("");
        button_export->setEnabled(false);
        return;
    }
    if(!string_name.endsWith(".sembcs"))
        string_name.append(".sembcs");
    delete sembcs_file;
    sembcs_file = new QFile(string_name);
    if(sembcs_file->exists())
        message->setText("existing boundary conditions will be overwritten");
    else
        message->setText("");
    button_export->setEnabled(true);
};

void ExportBoundaryConditionsDialog::browse()
{
    QFileDialog browse;
    browse.setFileMode(QFileDialog::AnyFile);
    browse.setDefaultSuffix("sembcs");
    browse.setNameFilter("SemSolver Boundary Conditions (*.sembcs)");
    browse.setWindowTitle("Browse...");
    browse.setAcceptMode(QFileDialog::AcceptOpen);
    if(!browse.exec())
        return;
    line_name->setText(browse.selectedFiles()[0]);
    checkName();
};
