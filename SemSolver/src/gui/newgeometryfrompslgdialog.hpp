#ifndef NEWGEOMETRYFROMPSLGDIALOG_HPP
#define NEWGEOMETRYFROMPSLGDIALOG_HPP

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

class NewGeometryFromPslgDialog : public QDialog
{
    Q_OBJECT

    // interface
    QGridLayout *layout;
    QLabel *label_poly_file;
    QLabel *poly_file_name;
    QLabel *message;
    QPushButton *button_browse;
    QLabel *label_name;
    QLineEdit *file_name;
    QPushButton *cancel;
    QPushButton *save;

    // variables
    QTemporaryFile semgeo_file;
    QFile *poly_file;

    const QStringList &geometries;

public:
    NewGeometryFromPslgDialog(QWidget *parent,
                              QStringList const &existing_geometries);
    ~NewGeometryFromPslgDialog();
    inline QFile *file()
    {
        return &semgeo_file;
    };
    inline QString name()
    {
        return file_name->text();
    };

private slots:
    void checkName();
    void saveFile();
    void browse();
};

#endif // NEWGEOMETRYFROMPSLGDIALOG_HPP
