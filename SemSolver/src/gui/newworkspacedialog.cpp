#include "newworkspacedialog.hpp"

NewWorkspaceDialog::NewWorkspaceDialog(QWidget *parent)
    : QFileDialog(parent)
{
    setFileMode(QFileDialog::AnyFile);
    setDefaultSuffix("semsol");
    setNameFilter("SemSolver Workspace (*.semsol)");
    setWindowTitle("New Workspace...");
    setAcceptMode(QFileDialog::AcceptSave);
    setConfirmOverwrite(true);
};
