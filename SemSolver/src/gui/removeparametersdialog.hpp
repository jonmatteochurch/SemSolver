#ifndef REMOVEPARAMETERSDIALOG_HPP
#define REMOVEPARAMETERSDIALOG_HPP

#include <QComboBox>
#include <QDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QWidget>

#include "../lib/semsolver-io/workspace.hpp"

class RemoveParametersDialog : public QDialog
{
	Q_OBJECT

		// interface
		QGridLayout *layout;
	QLabel *label_parameters;
	QComboBox *parameters_list;
	QLabel *message;
	QPushButton *cancel;
	QPushButton *button_remove;

	// variables
	const QStringList &parameters;
	QFile *const semsol_file;

public:
	RemoveParametersDialog(QWidget *parent,
		QStringList const &existing_parameters,
		QFile * const workspace);
	~RemoveParametersDialog();

private slots:
	inline void remove()
	{
		SemSolver::IO::remove_file_from_workspace(semsol_file, parameters_list->currentText() + ".semprm");
		done(true);
	};
};

#endif // REMOVEPARAMETERSDIALOG_HPP
