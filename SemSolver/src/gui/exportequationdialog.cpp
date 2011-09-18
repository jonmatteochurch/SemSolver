#include "exportequationdialog.hpp"

#include <QFileDialog>

ExportEquationDialog::ExportEquationDialog(
        QWidget *parent,
        QStringList &existing_equations,
        QFile *const workspace)
            : QDialog(parent), equations(existing_equations),
            semsol_file(workspace)
{
    layout = new QGridLayout(this);
    label_equation = new QLabel(this);
    equations_list = new QComboBox(this);
    message = new QLabel(this);
    label_name = new QLabel(this);
    line_name = new QLineEdit(this);
    button_browse = new QPushButton(this);
    cancel = new QPushButton(this);
    button_export = new QPushButton(this);
    label_equation->setText("Equation:");
    equations_list->addItems(equations);
    label_name->setText("Name:");
    button_browse->setText("&Browse");
    cancel->setText("&Cancel");
    button_export->setText("&Export");
    button_export->setEnabled(false);
    layout->addWidget(label_equation,0,0);
    layout->addWidget(equations_list,0,1,1,4);
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

    semeqn_file = 0;

    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Export Equation");
};

ExportEquationDialog::~ExportEquationDialog()
{
    delete layout;
    delete label_equation;
    delete equations_list;
    delete label_name;
    delete line_name;
    delete button_browse;
    delete cancel;
    delete button_export;
    delete semeqn_file;
}

void ExportEquationDialog::checkName()
{
    QString string_name = line_name->text();
    if(string_name.isEmpty())
    {
        message->setText("");
        button_export->setEnabled(false);
        return;
    }
    if(!string_name.endsWith(".semeqn"))
        string_name.append(".semeqn");
    delete semeqn_file;
    semeqn_file = new QFile(string_name);
    if(semeqn_file->exists())
        message->setText("existing equation will be overwritten");
    else
        message->setText("");
    button_export->setEnabled(true);
};

void ExportEquationDialog::browse()
{
    QFileDialog browse;
    browse.setFileMode(QFileDialog::AnyFile);
    browse.setDefaultSuffix("semeqn");
    browse.setNameFilter("SemSolver Equation (*.semeqn)");
    browse.setWindowTitle("Browse...");
    browse.setAcceptMode(QFileDialog::AcceptOpen);
    if(!browse.exec())
        return;
    line_name->setText(browse.selectedFiles()[0]);
    checkName();
};
