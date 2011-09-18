#ifndef NEWEQUATIONDIALOG_HPP
#define NEWEQUATIONDIALOG_HPP

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTemporaryFile>
#include <QVBoxLayout>
#include <QWidget>

#include "formula.hpp"

class NewEquationDialog : public QDialog
{
    Q_OBJECT

    // interface
    QVBoxLayout *layout;
    QHBoxLayout *layout_problem;
    QGridLayout *layout_inputs;
    QHBoxLayout *layout_buttons;
    QLabel *label_problem;
    QComboBox *problem_list;
    Formula *problem;
    Formula *label_diffusion;
    QLineEdit *diffusion;
    Formula *label_convection_x;
    QLineEdit *convection_x;
    Formula *label_convection_y;
    QLineEdit *convection_y;
    Formula *label_reaction;
    QLineEdit *reaction;
    Formula *label_source;
    QLineEdit *source;
    QLabel *message;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_save;

    // variables
    QTemporaryFile temp_file;
    const QStringList &equations;

public:
    NewEquationDialog(QWidget *parent,
                      QStringList const &existing_equations);
    ~NewEquationDialog();
    inline QTemporaryFile *file()
		{
    return &temp_file;
};
    inline QString name()
		{
    return line_name->text();
};

private slots:
    void checkInput();
    void checkName();
    void save();
};

#endif // NEWEQUATIONDIALOG_HPP
