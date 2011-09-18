#ifndef EXPORTEQUATIONDIALOG_HPP
#define EXPORTEQUATIONDIALOG_HPP

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

class ExportEquationDialog : public QDialog
{
    Q_OBJECT

    // interface
    QGridLayout *layout;
    QComboBox *equations_list;
    QLabel *message;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *button_browse;
    QLabel *label_equation;
    QPushButton *cancel;
    QPushButton *button_export;

    // variables
    QFile *semeqn_file;
    const QStringList &equations;
    QFile * const semsol_file;

public:
    ExportEquationDialog(QWidget *parent,
                         QStringList &existing_equations,
                         QFile * const workspace);
    ~ExportEquationDialog();
    inline QFile *file();

private slots:

    void checkName();
    void browse();
    inline void exportFile();
};

inline QFile *ExportEquationDialog::file()
{
    return semeqn_file;
};

inline void ExportEquationDialog::exportFile()
{
    SemSolver::IO::extract_file_from_workspace(semsol_file, equations_list->currentText() + ".semeqn",
                                                   semeqn_file);
    done(true);
};

#endif // EXPORTEQUATIONDIALOG_HPP
