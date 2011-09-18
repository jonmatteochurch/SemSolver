#ifndef IO_EQUATION_HPP
#define IO_EQUATION_HPP

#include <QFile>

#include <SemSolver/equation.hpp>

namespace SemSolver
{
    //! Namespace for Input/Output operations on SemSolver Classes
    namespace IO
    {
        //! Read 2D Equation from file
        template<class X>
        bool read_equation(QFile *file,
                           Equation<2, X> *&equation);
    };
};

#include <SemSolver/diffusionconvectionreactionequation.hpp>
#include <SemSolver/scriptfunction.hpp>

template<class X>
bool SemSolver::IO::read_equation(QFile *file,
                                  Equation<2, X> *&equation)
{
#ifdef SEMDEBUG
    if(!file->open(QIODevice::ReadOnly))
    {
        qWarning("SemSolver::IO::readEquation - ERROR : cannot open file.");
        return false;
    }
#else
    file->open(QIODevice::ReadOnly);
#endif
    QTextStream input(file);
    QStringList values;
    values = next_non_empty_line_values(input);
#ifdef SEMDEBUG
    if(values.isEmpty())
    {
        qWarning("SemSolver::IO::readEquation - ERROR : missing type value in file.");
        file->close();
        return false;
    }
    if(values.size()>1)
    {
        qWarning("SemSolver::IO::readEquation - ERROR : too many inputs in first line of"\
                 " file.");
        file->close();
        return false;
    }
#endif
    if(values[0]=="DIFFUSION_CONVECTION_REACTION")
    {
        DiffusionConvectionReactionEquation<2, X> *new_equation =
                new DiffusionConvectionReactionEquation<2, X>();

        values = next_non_empty_line_values(input);
        while(!values.isEmpty())
        {
            if(values[0]=="DIFFUSION")
            {
#ifdef SEMDEBUG
                if(values.size()!=2)
                {
                    qWarning("SemSolver::IO::readEquation - ERROR : wrong number of inpu"\
                             "ts on diffusion line.");
                    file->close();
                    return false;
                }
#endif
                new_equation->setDiffusion(
                        new ScriptFunction< Point<2, X>, X >(values[1]));
            }
            else if(values[0]=="CONVECTION")
            {
#ifdef SEMDEBUG
                if(values.size()!=3)
                {
                    qWarning("SemSolver::IO::readEquation - ERROR : wrong number of inpu"\
                             "ts on convection line.");
                    file->close();
                    return false;
                }
#endif
                new_equation->setConvection(
                        new ScriptFunction< Point<2, X>, Vector<X> >(values.mid(1, 2)));
            }
            else if(values[0]=="REACTION")
            {
#ifdef SEMDEBUG
                if(values.size()!=2)
                {
                    qWarning("SemSolver::IO::readEquation - ERROR : wrong number of inpu"\
                             "ts on reaction line.");
                    file->close();
                    return false;
                }
#endif
                new_equation->setReaction(new ScriptFunction< Point<2, X>, X >(values[1]));
            }
            else if(values[0]=="FORCING")
            {
#ifdef SEMDEBUG
                if(values.size()!=2)
                {
                    qWarning("SemSolver::IO::readEquation - ERROR : wrong number of inpu"\
                             "ts on forcing line.");
                    file->close();
                    return false;
                }
#endif
                new_equation->setForcing(new ScriptFunction< Point<2, X>, X >(values[1]));
            }
#ifdef SEMDEBUG
            else
            {
                qWarning("SemSolver::IO::readEquation - ERROR : unknown input line in fi"\
                         "le.");
                file->close();
                return false;
            };
#endif
            values = next_non_empty_line_values(input);
        }
        delete equation;
        equation = new_equation;
    }
#ifdef SEMDEBUG
    else
    {
        qWarning("SemSolver::IO::readEquation - ERROR : unknown type value in file.");
        file->close();
        return false;
    }
#endif
    file->close();

    return true;
};

#endif // IO_EQUATION_HPP
