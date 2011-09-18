#ifndef EXPORTPARAMETERSDIALOG_HPP
#define EXPORTPARAMETERSDIALOG_HPP

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

class ExportParametersDialog : public QDialog
{
    Q_OBJECT

    // interface
    QGridLayout *layout;
    QComboBox *parameters_list;
    QLabel *message;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *button_browse;
    QLabel *label_parameters;
    QPushButton *cancel;
    QPushButton *button_export;

    // variables
    QFile *semprm_file;
    const QStringList &parameters;
    QFile * const semsol_file;

public:
    ExportParametersDialog(QWidget *parent,
                         QStringList &existing_parameters,
                         QFile * const workspace);
    ~ExportParametersDialog();
    inline QFile *file()
		{
    return semprm_file;
};


private slots:

    void checkName();
    void browse();
    inline void exportFile()
		{
    SemSolver::IO::extract_file_from_workspace(semsol_file, parameters_list->currentText() + ".semprm",
                                                   semprm_file);
    done(true);
};
};

#endif // EXPORTPARAMETERSDIALOG_HPP
