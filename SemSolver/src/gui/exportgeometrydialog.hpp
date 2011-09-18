#ifndef EXPORTGEOMETRYDIALOG_HPP
#define EXPORTGEOMETRYDIALOG_HPP

#include <QComboBox>
#include <QDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QWidget>

#include "../lib/semsolver-io/workspace.hpp"

class ExportGeometryDialog : public QDialog
{
    Q_OBJECT

    // interface
    QGridLayout *layout;
    QComboBox *geometries_list;
    QLabel *message;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *button_browse;
    QLabel *label_geometry;
    QPushButton *cancel;
    QPushButton *button_export;

    // variables
    QFile *semgeo_file;
    const QStringList &geometries;
    QFile * const semsol_file;

public:
    ExportGeometryDialog(QWidget *parent,
                         QStringList &existing_geometries,
                         QFile * const workspace);
    ~ExportGeometryDialog();
    inline QFile *file()		
{
    return semgeo_file;
};

private slots:

    void checkName();
    void browse();
    inline void exportFile()
		{
    SemSolver::IO::extract_file_from_workspace(semsol_file, geometries_list->currentText() + ".semgeo",
                                                   semgeo_file);
    done(true);
};

};

#endif // EXPORTGEOMETRYDIALOG_HPP
