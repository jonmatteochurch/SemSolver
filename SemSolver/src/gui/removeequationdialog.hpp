#ifndef REMOVEEQUATIONDIALOG_HPP
#define REMOVEEQUATIONDIALOG_HPP

#include <QComboBox>
#include <QDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QWidget>

#include "../lib/semsolver-io/workspace.hpp"

class RemoveEquationDialog : public QDialog
{
    Q_OBJECT

    // interface
    QGridLayout *layout;
    QLabel *label_equation;
    QComboBox *equations_list;
    QLabel *message;
    QPushButton *cancel;
    QPushButton *button_remove;

    // variables
    const QStringList &equations;
    QFile *const semsol_file;


public:
    RemoveEquationDialog(QWidget *parent,
                         QStringList const &existing_equations,
                         QFile * const workspace);
    ~RemoveEquationDialog();

private slots:
    inline void remove()
	{
	    SemSolver::IO::remove_file_from_workspace(semsol_file, equations_list->currentText() + ".semeqn");
		done(true);
	};
};

#endif // REMOVEGEOMETRYDIALOG_HPP
