#ifndef IMPORTBOUNDARYCONDITIONSDIALOG_HPP
#define IMPORTBOUNDARYCONDITIONSDIALOG_HPP

#include <QDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QWidget>

class ImportBoundaryConditionsDialog : public QDialog
{
    Q_OBJECT
    // interface
    QGridLayout *layout;
    QLabel *label_sembcs_file;
    QLabel *sembcs_file_name;
    QLabel *message;
    QPushButton *button_browse;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_import;

    // variables
    QFile *sembcs_file;

    const QStringList &boundary_conditions;

public:
    ImportBoundaryConditionsDialog(QWidget *parent,
                         QStringList const &existing_boundary_conditions);
    ~ImportBoundaryConditionsDialog();
    inline QString name()
		{
    return line_name->text();
	};

    inline QFile *file()
		{
    return sembcs_file;
};


private slots:

    void checkName();
    void browse();
    inline void import()
		{
    done(true);
};

};

#endif // IMPORTBOUNDARYCONDITIONSDIALOG_HPP
