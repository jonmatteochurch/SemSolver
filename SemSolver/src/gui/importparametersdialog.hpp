#ifndef IMPORTPARAMETERSDIALOG_HPP
#define IMPORTPARAMETERSDIALOG_HPP

#include <QDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QWidget>

class ImportParametersDialog : public QDialog
{
    Q_OBJECT
    // interface
    QGridLayout *layout;
    QLabel *label_semprm_file;
    QLabel *semprm_file_name;
    QLabel *message;
    QPushButton *button_browse;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_import;

    // variables
    QFile *semprm_file;

    const QStringList &parameters;

public:
    ImportParametersDialog(QWidget *parent,
                         QStringList const &existing_parameters);
    ~ImportParametersDialog();
    inline QString name()
		{
    return line_name->text();
}

    inline QFile *file()
		{
    return semprm_file;
};


private slots:

    void checkName();
    void browse();
    inline void import()
		{
    done(true);
};

};

#endif // IMPORTPARAMETERSDIALOG_HPP
