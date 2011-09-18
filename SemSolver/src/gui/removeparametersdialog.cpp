#include "removeparametersdialog.hpp"

RemoveParametersDialog::RemoveParametersDialog(
        QWidget *parent,
        const QStringList &existing_parameters,
        QFile *const workspace)
            : QDialog(parent), parameters(existing_parameters), semsol_file(workspace)
{
    layout = new QGridLayout;
    label_parameters = new QLabel;
    parameters_list = new QComboBox;
    message = new QLabel;
    cancel = new QPushButton;
    button_remove = new QPushButton;
    label_parameters->setText("Parameters:");
    parameters_list->addItems(parameters);
    cancel->setText("&Cancel");
    button_remove->setText("&Remove");
    layout->addWidget(label_parameters,0,0);
    layout->addWidget(parameters_list,0,1,1,4);
    layout->addWidget(message,1,0,1,5);
    layout->addWidget(cancel,2,3);
    layout->addWidget(button_remove,2,4);
    this->setLayout(layout);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Remove Parameters");

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_remove, SIGNAL(clicked()), this, SLOT(remove()));
};

RemoveParametersDialog::~RemoveParametersDialog()
{
    delete layout;
    delete label_parameters;
    delete parameters_list;
    delete cancel;
    delete button_remove;
};
