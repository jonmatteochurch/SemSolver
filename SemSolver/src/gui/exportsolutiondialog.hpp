#ifndef EXPORTSOLUTIONDIALOG_HPP
#define EXPORTSOLUTIONDIALOG_HPP

#include <QFileDialog>
#include <QString>
#include <QWidget>

class ExportSolutionDialog
    : public QFileDialog
{
public:
    ExportSolutionDialog(QWidget *parent = 0);
    inline QString fileName() const
		{
    return this->selectedFiles()[0];
};

};

#endif // EXPORTSOLUTIONDIALOG_HPP
