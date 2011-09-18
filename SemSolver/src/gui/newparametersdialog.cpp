#include "newparametersdialog.hpp"

#include <QMessageBox>
#include <QTextStream>

NewParametersDialog::NewParametersDialog(
	QWidget *parent, 
        const QStringList &existing_parameters)
            : QDialog(parent), parameters(existing_parameters)
{
    layout = new QVBoxLayout(this);
    input_layout0 = new QHBoxLayout;
    input_layout1 = new QHBoxLayout;
    input_layout2 = new QHBoxLayout;
    degree_label = new QLabel(this);
    degree_value = new QLineEdit(this);
    tolerance_label = new QLabel(this);
    tolerance_value = new QLineEdit(this);
    penality_label = new QLabel(this);
    penality_value = new QLineEdit(this);
    degree_label->setText("<b>Degree</b>");
    tolerance_label->setText("<b>Tolerance</b>");
    penality_label->setText("<b>Penality</b>");
    input_layout0->addWidget(degree_label);
    input_layout0->addWidget(degree_value);
    input_layout1->addWidget(tolerance_label);
    input_layout1->addWidget(tolerance_value);
    input_layout2->addWidget(penality_label);
    input_layout2->addWidget(penality_value);
    message = new QLabel(this);
    message->setAlignment(Qt::AlignRight);
    message->setText("");
    bottom_widget = new QWidget(this);
    bottom_layout = new QHBoxLayout(bottom_widget);
    label = new QLabel(bottom_widget);
    line_name = new QLineEdit(bottom_widget);
    cancel = new QPushButton(bottom_widget);
    button_save = new QPushButton(bottom_widget);
    label->setText("Name:");
    cancel->setText("&Cancel");
    cancel->setFixedSize(60,30);
    button_save->setText("&Save");
    button_save->setFixedSize(60,30);
    button_save->setEnabled(false);
    bottom_layout->addWidget(label);
    bottom_layout->addWidget(line_name);
    bottom_layout->addWidget(cancel);
    bottom_layout->addWidget(button_save);
    bottom_widget->setLayout(bottom_layout);
    layout->addLayout(input_layout0);
    layout->addLayout(input_layout1);
    layout->addLayout(input_layout2);
    layout->addWidget(message);
    layout->addWidget(bottom_widget);
    this->setLayout(layout);

    connect(degree_value, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(tolerance_value, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(penality_value, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
};

NewParametersDialog::~NewParametersDialog()
{
    delete message;
    delete degree_label;
    delete degree_value;
    delete tolerance_label;
    delete tolerance_value;
    delete penality_label;
    delete penality_value;
    delete label;
    delete line_name;
    delete cancel;
    delete button_save;
    delete input_layout0;
    delete input_layout1;
    delete input_layout2;
    delete bottom_layout;
    delete bottom_widget;
    delete layout;
};

void NewParametersDialog::checkInput()
{
    if(degree_value->text().isEmpty() || tolerance_value->text().isEmpty() ||
       penality_value->text().isEmpty())
    {
        button_save->setEnabled(false);
        message->setText("");
        return;
    }

    // check if degree value is integer
    bool ok;
    int degree = degree_value->text().toInt(&ok);
    if (!ok || degree<1)
    {
        button_save->setEnabled(false);
        message->setText("Degree must be a positive integer");
        return;
    }

    //check tolerance value
    double tolerance = tolerance_value->text().toDouble(&ok);
    if (!ok || tolerance<=0)
    {
        button_save->setEnabled(false);
        message->setText("Tolerance number must be a positive floating point number");
        return;
    }

    //check penality value
    double penality = penality_value->text().toDouble(&ok);
    if (!ok || penality<=0)
    {
        button_save->setEnabled(false);
        message->setText("Penality number must be a positive floating point number");
        return;
    }

    // if all checks are succesfully check name
    QString stringName = line_name->text();
    if(stringName.isEmpty())
    {
        message->setText("");
        button_save->setEnabled(false);
    }
    else if(parameters.contains(stringName))
    {
        message->setText("Existing file will be overwritten");
        button_save->setEnabled(true);
    }
    else
    {
        message->setText("");
        button_save->setEnabled(true);
    }
};

void NewParametersDialog::save()
{
    if(!temp_file.open())
    {
        QMessageBox message;
        message.setText("Error saving Parameters.");
        message.exec();
        return;
    }
    int degree = degree_value->text().toInt();
    double tolerance = tolerance_value->text().toDouble();
    double penality = penality_value->text().toDouble();
    QTextStream out(&temp_file);
    out << "DEGREE    \t" + QString::number(degree) + "\n";
    out << "TOLERANCE \t" + QString::number(tolerance) + "\n";
    out << "PENALITY  \t" + QString::number(penality) + "\n";
    temp_file.close();
    done(true);
};

