#ifndef IMPORTEQUATIONDIALOG_HPP
#define IMPORTEQUATIONDIALOG_HPP

#include <QDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QWidget>

class ImportEquationDialog : public QDialog
{
    Q_OBJECT
    // interface
    QGridLayout *layout;
    QLabel *label_semeqn_file;
    QLabel *semeqn_file_name;
    QLabel *message;
    QPushButton *button_browse;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_import;

    // variables
    QFile *semeqn_file;

    const QStringList &equations;

public:
    ImportEquationDialog(QWidget *parent,
                         QStringList const &existing_equations);
    ~ImportEquationDialog();
    inline QString name()
		{
    return line_name->text();
}

    inline QFile *file()
		{
    return semeqn_file;
};


private slots:

    void checkName();
    void browse();
    inline void import()
		{
    done(true);
};
};

#endif // IMPORTEQUATIONDIALOG_HPP
