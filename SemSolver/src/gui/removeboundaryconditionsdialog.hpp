#ifndef REMOVEBOUNDARYCONDITIONSDIALOG_HPP
#define REMOVEBOUNDARYCONDITIONSDIALOG_HPP

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QWidget>

#include "../lib/semsolver-io/workspace.hpp"

class RemoveBoundaryConditionsDialog : public QDialog
{
    Q_OBJECT

    // interface
    QGridLayout *layout;
    QLabel *label_boundary_conditions;
    QComboBox *boundary_conditions_list;
    QLabel *message;
    QPushButton *cancel;
    QPushButton *button_remove;

    // variables
    const QStringList &boundary_conditions;
    QFile *const semsol_file;


public:
    RemoveBoundaryConditionsDialog(QWidget *parent,
                         QStringList const &existing_boundary_conditions,
                         QFile * const workspace);
    ~RemoveBoundaryConditionsDialog();

private slots:
    inline void remove()
	{
		SemSolver::IO::remove_file_from_workspace(semsol_file, boundary_conditions_list->currentText() + ".sembcs");
		done(true);
	};
};

#endif // REMOVEBOUNDARYCONDITIONSDIALOG_HPP