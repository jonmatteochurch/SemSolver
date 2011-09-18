#ifndef IMPORTGEOMETRYDIALOG_HPP
#define IMPORTGEOMETRYDIALOG_HPP

#include <QDialog>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QWidget>


class ImportGeometryDialog : public QDialog
{
    Q_OBJECT
    // interface
    QGridLayout *layout;
    QLabel *label_semgeom_file;
    QLabel *semgeom_file_name;
    QLabel *message;
    QPushButton *button_browse;
    QLabel *label_name;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *button_import;

    // variables
    QFile *semgeom_file;

    const QStringList &geometries;

public:
    ImportGeometryDialog(QWidget *parent,
                         QStringList const &existing_geometries);
    ~ImportGeometryDialog();
    inline QString name()
	{
    return line_name->text();
}

    inline QFile *file()
		{
    return semgeom_file;
};


private slots:

    void checkName();
    void browse();
    inline void import()
		{
    done(true);
};
};

#endif // IMPORTGEOMETRYDIALOG_HPP
