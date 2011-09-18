#include "importparametersfromworkspacedialog.hpp"

#include <QFileDialog>

ImportParametersFromWorkspaceDialog::ImportParametersFromWorkspaceDialog(
        QWidget *parent,
        const QStringList &existing_parameters)
            : QDialog(parent), parameters(existing_parameters)
{
    layout = new QGridLayout(this);
    label_semsol_file = new QLabel(this);
    semsol_file_name = new QLabel(this);
    button_browse = new QPushButton(this);
    label_parameters = new QLabel(this);
    parameters_list = new QComboBox(this);
    message = new QLabel(this);
    label_name = new QLabel(this);
    line_name = new QLineEdit(this);
    cancel = new QPushButton(this);
    button_import = new QPushButton(this);
    label_semsol_file->setText("Workspace:");
    semsol_file_name->setText("<i>none selected</i>");
    button_browse->setText("&Browse");
    label_parameters->setText("Parameters:");
    parameters_list->setEnabled(false);
    label_name->setText("Name:");
    cancel->setText("&Cancel");
    button_import->setText("&Import");
    button_import->setEnabled(false);
    layout->addWidget(label_semsol_file,0,0);
    layout->addWidget(semsol_file_name,0,1,1,2);
    layout->addWidget(button_browse,0,3);
    layout->addWidget(label_parameters,1,0);
    layout->addWidget(parameters_list,1,1,1,3);
    layout->addWidget(message,2,0,1,4);
    layout->addWidget(label_name,3,0);
    layout->addWidget(line_name,3,1);
    layout->addWidget(cancel,3,2);
    layout->addWidget(button_import,3,3);
    this->setLayout(layout);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Import Parameters From Workspace");

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_import, SIGNAL(clicked()), this, SLOT(import()));
    connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));
    connect(button_browse, SIGNAL(clicked()), this, SLOT(browse()));

    workspace = 0;
};

ImportParametersFromWorkspaceDialog::~ImportParametersFromWorkspaceDialog()
{
    delete layout;
    delete label_semsol_file;
    delete semsol_file_name;
    delete button_browse;
    delete label_parameters;
    delete parameters_list;
    delete label_name;
    delete line_name;
    delete cancel;
    delete button_import;
    delete workspace;
}

void ImportParametersFromWorkspaceDialog::checkName()
{
    QString stringName = line_name->text();
    if(stringName.isEmpty())
    {
        message->setText("");
        button_import->setEnabled(false);
        return;
    }
    else if(parameters.contains(stringName))
        message->setText("existing parameters will be overwritten");
    else
        message->setText("");
    if(workspace->exists()&&parameters_list->isEnabled())
        button_import->setEnabled(true);
}

void ImportParametersFromWorkspaceDialog::browse()
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
    QStringList parameters;
    SemSolver::IO::get_parameters_list_from_workspace(workspace, parameters);
    if(!parameters.size())
        parameters_list->setEnabled(false);
    else
    {
        parameters_list->setEnabled(true);
        parameters_list->addItems(parameters);
    }
    checkName();
};
