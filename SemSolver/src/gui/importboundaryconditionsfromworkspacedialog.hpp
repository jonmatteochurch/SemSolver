#ifndef IMPORTBOUNDARYCONDITIONSFROMWORKSPACEDIALOG_HPP
#define IMPORTBOUNDARYCONDITIONSfROMWORKSPACEDIALOG_HPP

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

class ImportBoundaryConditionsFromWorkspaceDialog : public QDialog
{
    Q_OBJECT
    // interface
    QGridLayout *layout;
    QLabel *label_semsol_file;
    QLabel *semsol_file_name;
    QPushButton *button_browse;
    QLabel *label_boundary_conditions;
    QComboBox *boundary_conditions_list;
    QLabel *message;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_import;

    // variables
    //! The file of the new geometry
    QTemporaryFile temp_file;
    QFile *workspace;

    const QStringList boundary_conditions;

public:

    ImportBoundaryConditionsFromWorkspaceDialog(QWidget *parent,
                                      QStringList const &existing_boundary_conditions);
    ~ImportBoundaryConditionsFromWorkspaceDialog();
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
                                            boundary_conditions_list->currentText()+".sembcs",
                                            &temp_file);
    done(true);
};
};

#endif // IMPORTBOUNDARYCONDITIONSFROMWORKSPACEDIALOG_HPP
