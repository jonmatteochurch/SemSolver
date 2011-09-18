#ifndef NEWWORKSPACEDIALOG_HPP
#define NEWWORKSPACEDIALOG_HPP

#include <QFileDialog>
#include <QString>
#include <QWidget>

class NewWorkspaceDialog
    : public QFileDialog
{
public:
    NewWorkspaceDialog(QWidget *parent = 0);
    inline QString fileName() const
		{
    return this->selectedFiles()[0];
};
};

#endif // NEWWORKSPACEDIALOG_HPP
