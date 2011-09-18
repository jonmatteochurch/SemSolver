#ifndef EXPORTPLOTDIALOG_HPP
#define EXPORTPLOTDIALOG_HPP

#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QWidget>

class ExportPlotDialog
    : public QFileDialog
{
    QStringList filters;
    QStringList formats;
public:
    ExportPlotDialog(QWidget *parent = 0);
    inline QString fileName() const
		{
    return selectedFiles()[0] + selectedNameFilter().right(5).left(4);
};

    inline QString format() const
		{
    return formats.at(filters.indexOf(selectedNameFilter()));
};
};

#endif // EXPORTPLOTDIALOG_HPP
