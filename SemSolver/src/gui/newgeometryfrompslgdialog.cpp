#include "newgeometryfrompslgdialog.hpp"

#include <QFileDialog>
#include <QMessageBox>

#include "geometrytolerancedialog.hpp"
#include "../lib/semsolver/pslg.hpp"
#include "../lib/semsolver/polygonation.hpp"
#include "../lib/semsolver-preprocessor/computepolygonationfrompslg.hpp"
/*
#include "../lib/semsolver-io/subdomains.hpp"
*/
#include "../lib/semsolver-io/geometry.hpp"


NewGeometryFromPslgDialog::NewGeometryFromPslgDialog(
        QWidget *parent,
        const QStringList &existing_geometries)
    : QDialog(parent), geometries(existing_geometries)
{
    layout = new QGridLayout(this);
    label_poly_file = new QLabel(this);
    poly_file_name = new QLabel(this);
    message = new QLabel(this);
    button_browse = new QPushButton(this);
    label_name = new QLabel(this);
    file_name = new QLineEdit(this);
    cancel = new QPushButton(this);
    save = new QPushButton(this);
    label_poly_file->setText("PSLG:");
    poly_file_name->setText("<i>none selected</i>");
    button_browse->setText("&Browse");
    label_name->setText("Name:");
    cancel->setText("&Cancel");
    save->setText("&Save");
    save->setEnabled(false);
    layout->addWidget(label_poly_file,0,0);
    layout->addWidget(poly_file_name,0,1,1,2);
    layout->addWidget(button_browse,0,3);
    layout->addWidget(message,1,0,1,4);
    layout->addWidget(label_name,2,0);
    layout->addWidget(file_name,2,1);
    layout->addWidget(cancel,2,2);
    layout->addWidget(save, 2, 3);
    this->setLayout(layout);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("New Geometry From PSLG...");

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(save, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(file_name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));
    connect(button_browse, SIGNAL(clicked()), this, SLOT(browse()));

    poly_file = 0;
};

NewGeometryFromPslgDialog::~NewGeometryFromPslgDialog()
{
    delete layout;
    delete label_poly_file;
    delete poly_file_name;
    delete message;
    delete button_browse;
    delete label_name;
    delete file_name;
    delete cancel;
    delete save;
    delete poly_file;
};

void NewGeometryFromPslgDialog::checkName()
{
    QString string_name = file_name->text();
    if(string_name.isEmpty())
    {
        message->setText("");
        save->setEnabled(false);
        return;
    }
    else if(geometries.contains(string_name))
        message->setText("Existing file will be overwritten");
    else
        message->setText("");
    if(poly_file->exists())
        save->setEnabled(true);
};

void NewGeometryFromPslgDialog::saveFile()
{
    SemSolver::PSLG<double> pslg;
    SemSolver::Polygonation<2, double> polygonation;
    if(!SemSolver::IO::read_PSLG(poly_file, pslg))
    {
        QMessageBox message;
        message.setText("Error PSLG file corrupted.");
        message.exec();
        return;
    }
    if(!SemSolver::PreProcessor::compute_polygonation_from_pslg(pslg, polygonation))
    {
        QMessageBox message;
        message.setText("Error. The PSLG does not describe a valid polygon with holes.");
        message.exec();
        return;
    }
    if(!polygonation.isQuadrangulation())
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        polygonation.refine();
    QTemporaryFile domains_file;
    SemSolver::IO::write_subdomains(polygonation, &domains_file);
    SemSolver::IO::write_geometry(poly_file, &domains_file, &semgeo_file);
    done(true);
};

void NewGeometryFromPslgDialog::browse()
{
    QFileDialog browse;
    browse.setFileMode(QFileDialog::ExistingFile);
    browse.setDefaultSuffix("poly");
    browse.setNameFilter("Triangle PSLG (*.poly)");
    browse.setWindowTitle("Browse...");
    browse.setAcceptMode(QFileDialog::AcceptOpen);
    if(!browse.exec())
        return;
    delete poly_file;
    poly_file = new QFile(browse.selectedFiles()[0]);
    poly_file_name->setText(poly_file->fileName());
    checkName();
};
