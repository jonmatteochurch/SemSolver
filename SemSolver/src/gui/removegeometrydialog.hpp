#ifndef REMOVEGEOMETRYDIALOG_HPP
#define REMOVEGEOMETRYDIALOG_HPP

#include <QComboBox>
#include <QDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QWidget>

#include "../lib/semsolver-io/workspace.hpp"

class RemoveGeometryDialog : public QDialog
{
    Q_OBJECT

    // interface
    QGridLayout *layout;
    QLabel *label_geometry;
    QComboBox *geometries_list;
    QLabel *message;
    QPushButton *cancel;
    QPushButton *button_remove;

    // variables
    const QStringList &geometries;
    QFile *const semsol_file;


public:
    RemoveGeometryDialog(QWidget *parent,
                         QStringList const &existing_geometries,
                         QFile * const workspace);
    ~RemoveGeometryDialog();

private slots:
    inline void remove()
	{
	    SemSolver::IO::remove_file_from_workspace(semsol_file, geometries_list->currentText() + ".semgeo");
		done(true);
	};
};

#endif // REMOVEGEOMETRYDIALOG_HPP
