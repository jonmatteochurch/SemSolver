#include "geometrytolerancedialog.hpp"


GeometryToleranceDialog::GeometryToleranceDialog(
        QWidget *parent)
            : QDialog(parent)
{
    layout = new QVBoxLayout(this);
    input_layout0 = new QHBoxLayout;
    tolerance_label = new QLabel(this);
    tolerance_value = new QLineEdit(this);
    tolerance_label->setText("<b>Tolerance</b>");
    input_layout0->addWidget(tolerance_label);
    input_layout0->addWidget(tolerance_value);
    message = new QLabel(this);
    message->setAlignment(Qt::AlignRight);
    message->setText("");
    bottom_widget = new QWidget(this);
    bottom_layout = new QHBoxLayout(bottom_widget);
    cancel = new QPushButton(bottom_widget);
    button_save = new QPushButton(bottom_widget);
    cancel->setText("&Cancel");
    cancel->setFixedSize(60,30);
    button_save->setText("&Set");
    button_save->setFixedSize(60,30);
    button_save->setEnabled(false);
    bottom_layout->addStretch();
    bottom_layout->addWidget(cancel);
    bottom_layout->addWidget(button_save);
    bottom_widget->setLayout(bottom_layout);
    layout->addLayout(input_layout0);
    layout->addWidget(message);
    layout->addWidget(bottom_widget);
    this->setLayout(layout);
    this->setFixedWidth(450);
    this->setWindowTitle("Set Geometry Tolerance...");

    connect(tolerance_value, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_save, SIGNAL(clicked()), this, SLOT(accept()));
};

GeometryToleranceDialog::~GeometryToleranceDialog()
{
    delete message;
    delete tolerance_label;
    delete tolerance_value;
    delete cancel;
    delete button_save;
    delete input_layout0;
    delete bottom_layout;
    delete bottom_widget;
    delete layout;
};

void GeometryToleranceDialog::checkInput() const
{
    if(tolerance_value->text().isEmpty())
    {
        button_save->setEnabled(false);
        message->setText("");
        return;
    }

    bool ok;
    //check tolerance value
    double tolerance = tolerance_value->text().toDouble(&ok);
    if (!ok || tolerance<=0)
    {
        button_save->setEnabled(false);
        message->setText("Tolerance number must be a positive floating point number");
        return;
    }
    button_save->setEnabled(true);
    message->setText("");
    return;
};
