#include "importboundaryconditionsfromworkspacedialog.hpp"

#include <QFileDialog>

ImportBoundaryConditionsFromWorkspaceDialog::ImportBoundaryConditionsFromWorkspaceDialog(
        QWidget *parent,
        const QStringList &existing_boundary_conditions)
            : QDialog(parent), boundary_conditions(existing_boundary_conditions)
{
    layout = new QGridLayout(this);
    label_semsol_file = new QLabel(this);
    semsol_file_name = new QLabel(this);
    button_browse = new QPushButton(this);
    label_boundary_conditions = new QLabel(this);
    boundary_conditions_list = new QComboBox(this);
    message = new QLabel(this);
    label_name = new QLabel(this);
    line_name = new QLineEdit(this);
    cancel = new QPushButton(this);
    button_import = new QPushButton(this);
    label_semsol_file->setText("Workspace:");
    semsol_file_name->setText("<i>none selected</i>");
    button_browse->setText("&Browse");
    label_boundary_conditions ->setText("Boundary Conditions:");
    boundary_conditions_list->setEnabled(false);
    label_name->setText("Name:");
    cancel->setText("&Cancel");
    button_import->setText("&Import");
    button_import->setEnabled(false);
    layout->addWidget(label_semsol_file,0,0);
    layout->addWidget(semsol_file_name,0,1,1,2);
    layout->addWidget(button_browse,0,3);
    layout->addWidget(label_boundary_conditions,1,0);
    layout->addWidget(boundary_conditions_list,1,1,1,3);
    layout->addWidget(message,2,0,1,4);
    layout->addWidget(label_name,3,0);
    layout->addWidget(line_name,3,1);
    layout->addWidget(cancel,3,2);
    layout->addWidget(button_import,3,3);
    this->setLayout(layout);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Import Boundary Conditions From Workspace");

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_import, SIGNAL(clicked()), this, SLOT(import()));
    connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));
    connect(button_browse, SIGNAL(clicked()), this, SLOT(browse()));

    workspace = 0;
};

ImportBoundaryConditionsFromWorkspaceDialog::~ImportBoundaryConditionsFromWorkspaceDialog()
{
    delete layout;
    delete label_semsol_file;
    delete semsol_file_name;
    delete button_browse;
    delete label_boundary_conditions;
    delete boundary_conditions_list;
    delete label_name;
    delete line_name;
    delete cancel;
    delete button_import;
    delete workspace;
}

void ImportBoundaryConditionsFromWorkspaceDialog::checkName()
{
    QString stringName = line_name->text();
    if(stringName.isEmpty())
    {
        message->setText("");
        button_import->setEnabled(false);
        return;
    }
    else if(boundary_conditions.contains(stringName))
        message->setText("existing boundary conditions will be overwritten");
    else
        message->setText("");
    if(workspace->exists()&&boundary_conditions_list->isEnabled())
        button_import->setEnabled(true);
}

void ImportBoundaryConditionsFromWorkspaceDialog::browse()
{
    QFileDialog browse;
    browse.setFileMode(QFileDialog::ExistingFile);
    browse.setDefaultSuffix("semsol");
    browse.setNameFilter("SemSolver Workspace (*.semsol)");
    browse.setWindowTitle("Browse...");
    browse.setAcceptMode(QFileDialog::AcceptOpen);
    if(!browse.exec())
        return;
    delete workspace;
    workspace = new QFile(browse.selectedFiles()[0]);
    semsol_file_name->setText(workspace->fileName());
    QStringList boundary_conditions;
    SemSolver::IO::get_boundary_conditions_list_from_workspace(workspace, boundary_conditions);
    if(!boundary_conditions.size())
        boundary_conditions_list->setEnabled(false);
    else
    {
        boundary_conditions_list->setEnabled(true);
        boundary_conditions_list->addItems(boundary_conditions);
    }
    checkName();
};
