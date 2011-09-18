#ifndef WORKSPACE_HPP
#define WORKSPACE_HPP

#include <QFile>
#include <QString>
#include <QStringList>

#include <SemSolver/semgeometry.hpp>
#include <SemSolver/equation.hpp>
#include <SemSolver/boundaryconditions.hpp>
#include <SemSolver/semparameters.hpp>

#include  <SemSolver/IO/geometry.hpp>
#include  <SemSolver/IO/equation.hpp>
#include  <SemSolver/IO/boundaryconditions.hpp>
#include  <SemSolver/IO/parameters.hpp>

namespace SemSolver
{
    //! Namespace for Input/Output operations on SemSolver Classes
    namespace IO
    {
        //! Get list of geometries in workspace
        bool get_geometries_list_from_workspace(QFile *file,
                                                QStringList &geometries);
        //! Get list of equations in workspace
        bool get_equations_list_from_workspace(QFile *file,
                                           QStringList &equations);
        //! Get list of boundary conditions in workspace
        bool get_boundary_conditions_list_from_workspace(QFile *file,
                                                    QStringList &boundary_conditions);
        //! Get list of parameters in workspace
        bool get_parameters_list_from_workspace(QFile *file,
                                            QStringList &parameters);
        //! Read a geometry from workspace
        template<class X>
        bool get_geometry_from_workspace(QFile *file,
                                      QString const &name,
                                      SemGeometry<2, X> &geometry);
        //! Read an equation from workspace
        template<class X>
        bool get_equation_from_workspace(QFile *file,
                                      QString const &name,
                                      Equation<2, X> *&equation);
        //! Read boundary conditions from workspace
        template<class X>
        bool get_boundary_conditions_from_workspace(QFile *file,
                                                QString const &name,
                                                BoundaryConditions<2, X> &bc);
        //! Read parameters from workspace
        template<class X>
        bool get_parameters_from_workspace(QFile *file,
                                        QString const &name,
                                        SemParameters<X> &bc);
        //! Extract an entry from workspace
        bool extract_file_from_workspace(QFile *workspace,
                                      QString const &name,
                                      QFile *file);
        //! Add an entry to workspace
        bool add_file_to_workspace(QFile *workspace,
                                QString const &name,
                                QFile *file);
        //! Remove an entry from workspace
        bool remove_file_from_workspace(QFile *workspace,
                                   QString const &name);
    };
};

template<class X>
bool SemSolver::IO::get_geometry_from_workspace(QFile *file,
                                             const QString &name,
                                             SemGeometry<2,X> &geometry)
{
    QTemporaryFile *temp_file = new QTemporaryFile();
    if(!extract_file_from_workspace(file, name + ".semgeo", temp_file))
    {
        delete temp_file;
        return false;
    }
    if(!read_geometry(temp_file, geometry))
    {
        delete temp_file;
        return false;
    }
    delete temp_file;
    return true;
};

template<class X>
bool SemSolver::IO::get_equation_from_workspace(QFile *file,
                                             const QString &name,
                                             Equation<2, X> *&equation)
{
    QTemporaryFile *temp_file = new QTemporaryFile();
    if(!extract_file_from_workspace(file, name + ".semeqn", temp_file))
    {
        delete temp_file;
        return false;
    }
    if(!read_equation(temp_file, equation))
    {
        delete temp_file;
        return false;
    }
    delete temp_file;
    return true;
};

template<class X>
bool SemSolver::IO::get_boundary_conditions_from_workspace(QFile *file,
                                                       const QString &name,
                                                       BoundaryConditions<2, X> &bc)
{
    QTemporaryFile *temp_file = new QTemporaryFile();
    if(!extract_file_from_workspace(file, name + ".sembcs", temp_file))
    {
        delete temp_file;
        return false;
    }
    if(!read_boundary_conditions(temp_file, bc))
    {
        delete temp_file;
        return false;
    }
    delete temp_file;
    return true;
};

template<class X>
bool SemSolver::IO::get_parameters_from_workspace(QFile *file,
                                               const QString &name,
                                               SemParameters<X> &parameters)
{
    QTemporaryFile *temp_file = new QTemporaryFile();
    if(!extract_file_from_workspace(file, name + ".semprm", temp_file))
    {
        delete temp_file;
        return false;
    }
    if(!read_parameters(temp_file, parameters))
    {
        delete temp_file;
        return false;
    }
    delete temp_file;
    return true;
};

#endif // WORKSPACE_HPP
