#include "importboundaryconditionsdialog.hpp"

#include <QFileDialog>

ImportBoundaryConditionsDialog::ImportBoundaryConditionsDialog(QWidget *parent,
                                           const QStringList &existing_boundary_conditions)
    : QDialog(parent), boundary_conditions(existing_boundary_conditions)
{
    layout = new QGridLayout(this);
    label_sembcs_file = new QLabel(this);
    sembcs_file_name = new QLabel(this);
    message = new QLabel(this);
    button_browse = new QPushButton(this);
    label_name = new QLabel(this);
    line_name = new QLineEdit(this);
    cancel = new QPushButton(this);
    button_import = new QPushButton(this);
    label_sembcs_file->setText("Boundary Conditions:");
    sembcs_file_name->setText("<i>none selected</i>");
    button_browse->setText("&Browse");
    label_name->setText("Name:");
    cancel->setText("&Cancel");
    button_import->setText("&Import");
    button_import->setEnabled(false);
    layout->addWidget(label_sembcs_file,0,0);
    layout->addWidget(sembcs_file_name,0,1,1,2);
    layout->addWidget(button_browse,0,3);
    layout->addWidget(message,1,0,1,4);
    layout->addWidget(label_name,2,0);
    layout->addWidget(line_name,2,1);
    layout->addWidget(cancel,2,2);
    layout->addWidget(button_import,2,3);
    this->setLayout(layout);

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_import, SIGNAL(clicked()), this, SLOT(import()));
    connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));
    connect(button_browse, SIGNAL(clicked()), this, SLOT(browse()));

    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Import Boundary Conditions");

    sembcs_file = 0;
};

ImportBoundaryConditionsDialog::~ImportBoundaryConditionsDialog()
{
    delete layout;
    delete label_sembcs_file;
    delete sembcs_file_name;
    delete message;
    delete button_browse;
    delete label_name;
    delete line_name;
    delete cancel;
    delete button_import;
};

void ImportBoundaryConditionsDialog::checkName()
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
    if(sembcs_file->exists())
        button_import->setEnabled(true);
};

void ImportBoundaryConditionsDialog::browse()
{
    QFileDialog browse;
    browse.setFileMode(QFileDialog::ExistingFile);
    browse.setDefaultSuffix("sembcs");
    browse.setNameFilter("SemSolver Boundary Conditions (*.sembcs)");
    browse.setWindowTitle("Browse...");
    browse.setAcceptMode(QFileDialog::AcceptOpen);
    if(!browse.exec())
        return;
    delete sembcs_file;
    sembcs_file = new QFile(browse.selectedFiles()[0]);
    sembcs_file_name->setText(sembcs_file->fileName());
    checkName();
};
