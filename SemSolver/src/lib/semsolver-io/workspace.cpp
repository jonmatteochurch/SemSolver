#include "workspace.hpp"
#include "archive.hpp"

#include <QTemporaryFile>

bool SemSolver::IO::get_geometries_list_from_workspace(QFile *file,
                                                   QStringList &geometries)
{
    if(!file->exists())
        return false;
    geometries.clear();
    Archive archive(file);
    if(!archive.openRead())
        return false;
    QStringList entries = archive.entries();
    if(!archive.closeRead())
        return false;
    for (QStringList::Iterator it=entries.begin(); it!=entries.end(); ++it)
        if (it->endsWith(".semgeo"))
            geometries.push_back(it->section(".",0,-2));
    return true;
};

bool SemSolver::IO::get_equations_list_from_workspace(QFile *file,
                                                  QStringList &equations)
{
    if(!file->exists())
        return false;
    equations.clear();
    Archive archive(file);
    if(!archive.openRead())
        return false;
    QStringList entries = archive.entries();
    if(!archive.closeRead())
        return false;
    for (QStringList::Iterator it=entries.begin(); it!=entries.end(); ++it)
        if (it->endsWith(".semeqn"))
            equations.push_back(it->section(".",0,-2));
    return true;
};

bool SemSolver::IO::get_boundary_conditions_list_from_workspace
        (QFile *file,
         QStringList &boundary_conditions)
{
    if(!file->exists())
        return false;
    boundary_conditions.clear();
    Archive archive(file);
    if(!archive.openRead())
        return false;
    QStringList entries = archive.entries();
    if(!archive.closeRead())
        return false;
    for (QStringList::Iterator it=entries.begin(); it!=entries.end(); ++it)
        if (it->endsWith(".sembcs"))
            boundary_conditions.push_back(it->section(".",0,-2));
    return true;
};

bool SemSolver::IO::get_parameters_list_from_workspace(QFile *file,
                                                   QStringList &parameters)
{
    if(!file->exists())
        return false;
    parameters.clear();
    Archive archive(file);
    if(!archive.openRead())
        return false;
    QStringList entries = archive.entries();
    if(!archive.closeRead())
        return false;
    for (QStringList::Iterator it=entries.begin(); it!=entries.end(); ++it)
        if (it->endsWith(".semprm"))
            parameters.push_back(it->section(".",0,-2));
    return true;
};

bool SemSolver::IO::extract_file_from_workspace(QFile *workspace,
                                             const QString &name,
                                             QFile *file)
{
    if(!workspace->exists())
        return false;
    Archive archive(workspace);
    if(!archive.openRead())
        return false;
    if(!archive.extractFile(name, file))
        return false;
    if(!archive.closeRead())
        return false;
    return true;
};

bool SemSolver::IO::add_file_to_workspace(QFile *workspace,
                                       const QString &name,
                                       QFile *file)
{
    if(!workspace->exists() || !file->exists())
        return false;
    Archive archive(workspace);
    if(!archive.openRead())
        return false;
    QStringList list = archive.entries();
    list.removeAll(name);
    QTemporaryFile *temp_files = new QTemporaryFile[list.size()];
    for (int i=0; i<list.size(); ++i)
    {
        if(!archive.extractFile(list[i], temp_files+i))
        {
            delete[] temp_files;
            return false;
        }
    }
    if(!archive.closeRead())
    {
        delete[] temp_files;
        return false;
    }
    if(!archive.openWrite())
    {
        delete[] temp_files;
        return false;
    }
    for (int i=0; i<list.size(); ++i)
    {
        if(!archive.addFile(temp_files+i, list[i]))
        {
            delete[] temp_files;
            return false;
        }
    }
    delete[] temp_files;
    if(!archive.addFile(file, name))
        return false;
    if(!archive.closeWrite())
        return false;
    return true;
};

bool SemSolver::IO::remove_file_from_workspace(QFile *workspace,
                                       const QString &name)
{
    if(!workspace->exists())
        return false;
    Archive archive(workspace);
    if(!archive.openRead())
        return false;
    QStringList list = archive.entries();
    list.removeAll(name);
    QTemporaryFile *temp_files = new QTemporaryFile[list.size()];
    for (int i=0; i<list.size(); ++i)
    {
        if(!archive.extractFile(list[i], temp_files+i))
        {
            delete[] temp_files;
            return false;
        }
    }
    if(!archive.closeRead())
    {
        delete[] temp_files;
        return false;
    }
    if(!archive.openWrite())
    {
        delete[] temp_files;
        return false;
    }
    for (int i=0; i<list.size(); ++i)
    {
        if(!archive.addFile(temp_files+i, list[i]))
        {
            delete[] temp_files;
            return false;
        }
    }
    delete[] temp_files;
    if(!archive.closeWrite())
        return false;
    return true;
};
