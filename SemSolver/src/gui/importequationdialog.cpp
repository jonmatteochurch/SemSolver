#include "importequationdialog.hpp"

#include <QFileDialog>

ImportEquationDialog::ImportEquationDialog(QWidget *parent,
                                           const QStringList &existing_equations)
    : QDialog(parent), equations(existing_equations)
{
    layout = new QGridLayout(this);
    label_semeqn_file = new QLabel(this);
    semeqn_file_name = new QLabel(this);
    message = new QLabel(this);
    button_browse = new QPushButton(this);
    label_name = new QLabel(this);
    line_name = new QLineEdit(this);
    cancel = new QPushButton(this);
    button_import = new QPushButton(this);
    label_semeqn_file->setText("Equation:");
    semeqn_file_name->setText("<i>none selected</i>");
    button_browse->setText("&Browse");
    label_name->setText("Name:");
    cancel->setText("&Cancel");
    button_import->setText("&Import");
    button_import->setEnabled(false);
    layout->addWidget(label_semeqn_file,0,0);
    layout->addWidget(semeqn_file_name,0,1,1,2);
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
    this->setWindowTitle("Import Equation");

    semeqn_file = 0;
};

ImportEquationDialog::~ImportEquationDialog()
{
    delete layout;
    delete label_semeqn_file;
    delete semeqn_file_name;
    delete message;
    delete button_browse;
    delete label_name;
    delete line_name;
    delete cancel;
    delete button_import;
};

void ImportEquationDialog::checkName()
{
    QString stringName = line_name->text();
    if(stringName.isEmpty())
    {
        message->setText("");
        button_import->setEnabled(false);
        return;
    }
    else if(equations.contains(stringName))
        message->setText("existing equation will be overwritten");
    else
        message->setText("");
    if(semeqn_file->exists())
        button_import->setEnabled(true);
};

void ImportEquationDialog::browse()
{
    QFileDialog browse;
    browse.setFileMode(QFileDialog::ExistingFile);
    browse.setDefaultSuffix("semeqn");
    browse.setNameFilter("SemSolver Equation (*.semeqn)");
    browse.setWindowTitle("Browse...");
    browse.setAcceptMode(QFileDialog::AcceptOpen);
    if(!browse.exec())
        return;
    delete semeqn_file;
    semeqn_file = new QFile(browse.selectedFiles()[0]);
    semeqn_file_name->setText(semeqn_file->fileName());
    checkName();
};

