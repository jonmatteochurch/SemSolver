#ifndef IMPORTEQUATIONFROMWORKSPACEDIALOG_HPP
#define IMPORTEQUATIONFROMWORKSPACEDIALOG_HPP

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

class ImportEquationFromWorkspaceDialog : public QDialog
{
    Q_OBJECT
    // interface
    QGridLayout *layout;
    QLabel *label_semsol_file;
    QLabel *semsol_file_name;
    QPushButton *button_browse;
    QLabel *label_equation;
    QComboBox *equations_list;
    QLabel *message;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_import;

    // variables
    //! The file of the new geometry
    QTemporaryFile temp_file;
    QFile *workspace;

    const QStringList equations;

public:

    ImportEquationFromWorkspaceDialog(QWidget *parent,
                                      QStringList const &existing_equations);
    ~ImportEquationFromWorkspaceDialog();
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
                                            equations_list->currentText()+".semeqn",
                                            &temp_file);
    done(true);
};
};

#endif // IMPORTEQUATIONFROMWORKSPACEDIALOG_HPP
