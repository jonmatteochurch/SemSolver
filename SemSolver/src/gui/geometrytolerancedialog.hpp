#ifndef GEOMETRYTOLERANCEDIALOG_HPP
#define GEOMETRYTOLERANCEDIALOG_HPP

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class GeometryToleranceDialog : public QDialog
{
    Q_OBJECT

    // interface
    QVBoxLayout *layout;
    QLabel *message;
    QHBoxLayout *input_layout0;
    QLabel *tolerance_label;
    QLineEdit *tolerance_value;
    QWidget *bottom_widget;
    QHBoxLayout *bottom_layout;
    QPushButton *cancel;
    QPushButton *button_save;

public:
    GeometryToleranceDialog(QWidget *parent);

    ~GeometryToleranceDialog();
    inline double value() const
{
    return tolerance_value->text().toDouble();
};


private slots:
    void checkInput() const;
};

#endif // NEWPARAMETERSDIALOG_HPP
