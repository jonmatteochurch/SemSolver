#ifndef EXPORTBOUNDARYCONDITIONSDIALOG_HPP
#define EXPORTBOUNDARYCONDITIONSDIALOG_HPP

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

class ExportBoundaryConditionsDialog : public QDialog
{
    Q_OBJECT

    // interface
    QGridLayout *layout;
    QComboBox *boundary_conditions_list;
    QLabel *message;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *button_browse;
    QLabel *label_boundary_conditions;
    QPushButton *cancel;
    QPushButton *button_export;

    // variables
    QFile *sembcs_file;
    const QStringList &boundary_conditions;
    QFile * const semsol_file;

public:
    ExportBoundaryConditionsDialog(QWidget *parent,
                         QStringList &existing_boundary_conditions,
                         QFile * const workspace);
    ~ExportBoundaryConditionsDialog();
    inline QFile *file();

private slots:

    void checkName();
    void browse();
    inline void exportFile();
};

inline QFile *ExportBoundaryConditionsDialog::file()
{
    return sembcs_file;
};

inline void ExportBoundaryConditionsDialog::exportFile()
{
    SemSolver::IO::extract_file_from_workspace(semsol_file, boundary_conditions_list->currentText() + ".sembcs",
                                                   sembcs_file);
    done(true);
};

#endif // EXPORTBOUNDARYCONDITIONSDIALOG_HPP
