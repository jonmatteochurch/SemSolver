#include "removegeometrydialog.hpp"

RemoveGeometryDialog::RemoveGeometryDialog(
        QWidget *parent,
        const QStringList &existing_geometries,
        QFile *const workspace)
            : QDialog(parent), geometries(existing_geometries), semsol_file(workspace)
{
    layout = new QGridLayout;
    label_geometry = new QLabel;
    geometries_list = new QComboBox;
    message = new QLabel;
    cancel = new QPushButton;
    button_remove = new QPushButton;
    label_geometry->setText("Geometry:");
    geometries_list->addItems(geometries);
    cancel->setText("&Cancel");
    button_remove->setText("&Remove");
    layout->addWidget(label_geometry,0,0);
    layout->addWidget(geometries_list,0,1,1,4);
    layout->addWidget(message,1,0,1,5);
    layout->addWidget(cancel,2,3);
    layout->addWidget(button_remove,2,4);
    this->setLayout(layout);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Remove Geometry");

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_remove, SIGNAL(clicked()), this, SLOT(remove()));
};

RemoveGeometryDialog::~RemoveGeometryDialog()
{
    delete layout;
    delete label_geometry;
    delete geometries_list;
    delete cancel;
    delete button_remove;
};
