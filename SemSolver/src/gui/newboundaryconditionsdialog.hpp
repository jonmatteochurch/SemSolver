#ifndef NEWBOUNDARYCONDITIONSDIALOG_HPP
#define NEWBOUNDARYCONDITIONSDIALOG_HPP

#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QPushButton>
#include <QStringList>
#include <QTableWidget>
#include <QTemporaryFile>
#include <QVBoxLayout>
#include <QWidget>

#include "formula.hpp"

class NewBoundaryConditionsDialog : public QDialog
{
    Q_OBJECT

    // interface
    QVBoxLayout *layout;
    QTableWidget *table;
    QLabel *message;
    QHBoxLayout *input_layout0;
    QHBoxLayout *input_layout1;
    QHBoxLayout *input_layout2;
    QLabel *border_label;
    QLineEdit *border_value;
    QLabel *condition_label;
    QComboBox *conditions_list;
    Formula *condition;
    Formula *label0;
    QLineEdit *value0;
    Formula *label1;
    QLineEdit *value1;
    QPushButton *add_mod;
    QPushButton *del;
    QWidget *bottom_widget;
    QHBoxLayout *bottom_layout;
    QLabel *label;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_save;

    // variables
    QTemporaryFile temp_file;
    const QStringList boundary_conditions;
    int selected_row;
    QList<int> borders;

public:
    NewBoundaryConditionsDialog(
            QWidget *parent,
            QStringList const &existing_boundary_conditions);

    ~NewBoundaryConditionsDialog();
    inline QTemporaryFile *file()
		{
    return &temp_file;
};
    inline QString name()
		{
    return line_name->text();
};

private slots:
    void setBoundaryCondition(const int &index);
    void checkInput();
    void addModifyInput();
    void deleteInput();
    void save();
    void selectRow();
    void checkName();
};

#endif // NEWBOUNDARYCONDITIONSDIALOG_HPP
