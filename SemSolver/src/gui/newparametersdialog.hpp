#ifndef NEWPARAMETERSDIALOG_HPP
#define NEWPARAMETERSDIALOG_HPP

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTemporaryFile>
#include <QVBoxLayout>
#include <QWidget>

class NewParametersDialog : public QDialog
{
    Q_OBJECT

    // interface
    QVBoxLayout *layout;
    QLabel *message;
    QHBoxLayout *input_layout0;
    QHBoxLayout *input_layout1;
    QHBoxLayout *input_layout2;
    QLabel *degree_label;
    QLineEdit *degree_value;
    QLabel *tolerance_label;
    QLineEdit *tolerance_value;
    QLabel *penality_label;
    QLineEdit *penality_value;
    QWidget *bottom_widget;
    QHBoxLayout *bottom_layout;
    QLabel *label;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_save;

    // variables
    QTemporaryFile temp_file;
    const QStringList parameters;

public:
    NewParametersDialog(
            QWidget *parent,
            QStringList const &existing_parameters);

    ~NewParametersDialog();
    inline QTemporaryFile *file()
		{
    return &temp_file;
};
    inline QString name(){
    return line_name->text();
};

private slots:
    void checkInput();
    void save();
};

#endif // NEWPARAMETERSDIALOG_HPP
