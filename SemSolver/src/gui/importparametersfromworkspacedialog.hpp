#ifndef IMPORTPARAMETERSFROMWORKSPACEDIALOG_HPP
#define IMPORTPARAMETERSfROMWORKSPACEDIALOG_HPP

#include <QComboBox>
#include <QDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTemporaryFile>
#include <QWidget>

#include "../lib/semsolver-io/workspace.hpp"

class ImportParametersFromWorkspaceDialog : public QDialog
{
    Q_OBJECT
    // interface
    QGridLayout *layout;
    QLabel *label_semsol_file;
    QLabel *semsol_file_name;
    QPushButton *button_browse;
    QLabel *label_parameters;
    QComboBox *parameters_list;
    QLabel *message;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_import;

    // variables
    //! The file of the new geometry
    QTemporaryFile temp_file;
    QFile *workspace;

    const QStringList parameters;

public:

    ImportParametersFromWorkspaceDialog(QWidget *parent,
                                      QStringList const &existing_parameters);
    ~ImportParametersFromWorkspaceDialog();
    inline QString name()
		{
    return line_name->text();
};

    inline QFile *file()
		{
    return &temp_file;
};


private slots:

    void checkName();
    void browse();
    inline void import()
		{
    SemSolver::IO::extract_file_from_workspace(workspace,
                                            parameters_list->currentText()+".semprm",
                                            &temp_file);
    done(true);
};

};

#endif // IMPORTPARAMETERSFROMWORKSPACEDIALOG_HPP
