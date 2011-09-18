#include "exportsolutiondialog.hpp"

ExportSolutionDialog::ExportSolutionDialog(QWidget *parent)
    : QFileDialog(parent)
{
    setFileMode(QFileDialog::AnyFile);
    setDefaultSuffix("semsln");
    setNameFilter("SemSolver Solution (*.semsln)");
    setWindowTitle("Export Solution");
    setAcceptMode(QFileDialog::AcceptSave);
    setConfirmOverwrite(true);
};
