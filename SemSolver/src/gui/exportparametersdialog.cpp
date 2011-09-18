#include "exportparametersdialog.hpp"

#include <QFileDialog>

ExportParametersDialog::ExportParametersDialog(
        QWidget *parent,
        QStringList &existing_parameters,
        QFile *const workspace)
            : QDialog(parent), parameters(existing_parameters),
            semsol_file(workspace)
{
    layout = new QGridLayout(this);
    label_parameters = new QLabel(this);
    parameters_list = new QComboBox(this);
    message = new QLabel(this);
    label_name = new QLabel(this);
    line_name = new QLineEdit(this);
    button_browse = new QPushButton(this);
    cancel = new QPushButton(this);
    button_export = new QPushButton(this);
    label_parameters->setText("Parameters:");
    parameters_list->addItems(parameters);
    label_name->setText("Name:");
    button_browse->setText("&Browse");
    cancel->setText("&Cancel");
    button_export->setText("&Export");
    button_export->setEnabled(false);
    layout->addWidget(label_parameters,0,0);
    layout->addWidget(parameters_list,0,1,1,4);
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

    semprm_file = 0;

    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Export Parameters");
};

ExportParametersDialog::~ExportParametersDialog()
{
    delete layout;
    delete label_parameters;
    delete parameters_list;
    delete label_name;
    delete line_name;
    delete button_browse;
    delete cancel;
    delete button_export;
    delete semprm_file;
}

void ExportParametersDialog::checkName()
{
    QString string_name = line_name->text();
    if(string_name.isEmpty())
    {
        message->setText("");
        button_export->setEnabled(false);
        return;
    }
    if(!string_name.endsWith(".semprm"))
        string_name.append(".semprm");
    delete semprm_file;
    semprm_file = new QFile(string_name);
    if(semprm_file->exists())
        message->setText("existing parameters will be overwritten");
    else
        message->setText("");
    button_export->setEnabled(true);
};

void ExportParametersDialog::browse()
{
    QFileDialog browse;
    browse.setFileMode(QFileDialog::AnyFile);
    browse.setDefaultSuffix("semprm");
    browse.setNameFilter("SemSolver Parameters (*.semprm)");
    browse.setWindowTitle("Browse...");
    browse.setAcceptMode(QFileDialog::AcceptOpen);
    if(!browse.exec())
        return;
    line_name->setText(browse.selectedFiles()[0]);
    checkName();
};
