#include "openworkspacedialog.hpp"

OpenWorkspaceDialog::OpenWorkspaceDialog(QWidget *parent)
    : QFileDialog(parent)
{
    setFileMode(QFileDialog::ExistingFile);
    setDefaultSuffix("semsol");
    setNameFilter("SemSolver Workspace (*.semsol)");
    setWindowTitle("Open Workspace...");
    setAcceptMode(QFileDialog::AcceptOpen);
};
