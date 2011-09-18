#ifndef OPENWORKSPACEDIALOG_HPP
#define OPENWORKSPACEDIALOG_HPP

#include <QFileDialog>
#include <QString>
#include <QWidget>

class OpenWorkspaceDialog
    : public QFileDialog
{
public:
    OpenWorkspaceDialog(QWidget *parent = 0);
    inline QString fileName() const
		{
    return this->selectedFiles()[0];
};
};

#endif // OPENWORKSPACEDIALOG_HPP
