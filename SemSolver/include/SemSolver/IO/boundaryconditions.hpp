#ifndef IO_BOUNDARYCONDITIONS_HPP
#define IO_BOUNDARYCONDITIONS_HPP

#include <QFile>

#include <SemSolver/boundaryconditions.hpp>

namespace SemSolver
{
    //! Namespace for Input/Output operations on SemSolver Classes
    namespace IO
    {
        //! Read 2D BoundaryConditions from file
        template<class X>
        bool read_boundary_conditions(QFile *file,
                                    BoundaryConditions<2, X> &boundary_conditions);
    };
};

#include <SemSolver/scriptfunction.hpp>

template<class X>
bool SemSolver::IO::read_boundary_conditions(QFile *file,
                                           BoundaryConditions<2, X> &bc)
{
    bc.clear();
#ifdef SEMDEBUG
    if(!file->open(QIODevice::ReadOnly))
    {
        qWarning("SemSolver::IO::readBoundaryCondition - ERROR : cannot open file.");
        return false;
    }
#endif
    QTextStream input(file);
    QStringList values;
    values = next_non_empty_line_values(input);
    while(!values.isEmpty())
    {
#ifdef SEMDEBUG
        if(values.size()<2)
        {
            qWarning("SemSolver::IO::readBoundaryCondition - ERROR : wrong number of inp"\
                     "uts on a line.");
            file->close();
            return false;
        }
#endif
        if(values[1]=="DIRICHLET")
        {
#ifdef SEMDEBUG
            if(values.size()!=3)
            {
                qWarning("SemSolver::IO::readBoundaryCondition - ERROR : wrong number of"\
                         "inputs on Dirichlet line.");
                file->close();
                return false;
            }
#endif
            bc.setBorder(values[0].toInt(),
                         BoundaryConditions<2,X>::DIRICHLET,
                         new ScriptFunction< Point<2, X>, X >(values[2]));
        }
        else if(values[1]=="NEUMANN")
        {
#ifdef SEMDEBUG
            if(values.size()!=3)
            {
                qWarning("SemSolver::IO::readBoundaryCondition - ERROR : wrong number of"\
                         "inputs on Neumann line.");
                file->close();
                return false;
            }
#endif
            bc.setBorder(values[0].toInt(),
                         BoundaryConditions<2,X>::NEUMANN,
                         new ScriptFunction< Point<2, X>, X >(values[2]));
        }
        else if(values[1]=="ROBIN")
        {
#ifdef SEMDEBUG
            if(values.size()!=4)
            {
                qWarning("SemSolver::IO::readBoundaryCondition - ERROR : wrong number of"\
                         "inputs on Robin line.");
                file->close();
                return false;
            }
#endif
            bc.setBorder(values[0].toInt(),
                         BoundaryConditions<2,X>::ROBIN,
                         new ScriptFunction< Point<2, X>, X >(values[2]),
                         new ScriptFunction< Point<2, X>, X >(values[3]));
        }
#ifdef SEMDEBUG
        else
        {
            qWarning("SemSolver::IO::readBoundaryCondition - ERROR : unknown input line "\
                     "in file.");
            file->close();
            return false;
        };
#endif
        values = next_non_empty_line_values(input);
    }
    file->close();

    return true;
};

#endif // IO_BOUNDARYCONDITIONS_HPP
