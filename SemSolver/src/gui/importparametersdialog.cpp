#include "importparametersdialog.hpp"

#include <QFileDialog>

ImportParametersDialog::ImportParametersDialog(QWidget *parent,
                                           const QStringList &existing_parameters)
    : QDialog(parent), parameters(existing_parameters)
{
    layout = new QGridLayout(this);
    label_semprm_file = new QLabel(this);
    semprm_file_name = new QLabel(this);
    message = new QLabel(this);
    button_browse = new QPushButton(this);
    label_name = new QLabel(this);
    line_name = new QLineEdit(this);
    cancel = new QPushButton(this);
    button_import = new QPushButton(this);
    label_semprm_file->setText("Parameters:");
    semprm_file_name->setText("<i>none selected</i>");
    button_browse->setText("&Browse");
    label_name->setText("Name:");
    cancel->setText("&Cancel");
    button_import->setText("&Import");
    button_import->setEnabled(false);
    layout->addWidget(label_semprm_file,0,0);
    layout->addWidget(semprm_file_name,0,1,1,2);
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
    this->setWindowTitle("Import Parameters");

    semprm_file = 0;
};

ImportParametersDialog::~ImportParametersDialog()
{
    delete layout;
    delete label_semprm_file;
    delete semprm_file_name;
    delete message;
    delete button_browse;
    delete label_name;
    delete line_name;
    delete cancel;
    delete button_import;
};

void ImportParametersDialog::checkName()
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
    if(semprm_file->exists())
        button_import->setEnabled(true);
};

void ImportParametersDialog::browse()
{
    QFileDialog browse;
    browse.setFileMode(QFileDialog::ExistingFile);
    browse.setDefaultSuffix("semprm");
    browse.setNameFilter("SemSolver Parameters (*.semprm)");
    browse.setWindowTitle("Browse...");
    browse.setAcceptMode(QFileDialog::AcceptOpen);
    if(!browse.exec())
        return;
    delete semprm_file;
    semprm_file = new QFile(browse.selectedFiles()[0]);
    semprm_file_name->setText(semprm_file->fileName());
    checkName();
};

