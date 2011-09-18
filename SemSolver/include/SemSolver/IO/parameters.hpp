#ifndef IO_PARAMETERS_HPP
#define IO_PARAMETERS_HPP

#include <QFile>
#include <QTextStream>

#include <SemSolver/semparameters.hpp>

namespace SemSolver
{
    //! Namespace for Input/Output operations on SemSolver Classes
    namespace IO
    {
        //! Read SemParameters from file
        template<class X>
        bool read_parameters(QFile *file,
                            SemParameters<X> &parameters);
    };
};


template<class X>
bool SemSolver::IO::read_parameters(QFile *file,
                                   SemParameters<X> &parameters)
{
    bool degree = false, tolerance = false, penality = false;
#ifdef SEMDEBUG
    if(!file->open(QIODevice::ReadOnly))
    {
        qWarning("SemSolver::IO::readParameters - ERROR : cannot open file.");
        return false;
    }
#else
    file->open(QIODevice::ReadOnly);
#endif
    QTextStream input(file);
    QStringList values;
    values = next_non_empty_line_values(input);
    while(!values.isEmpty())
    {
        if(values[0]=="DEGREE")
        {
#ifdef SEMDEBUG
            if(values.size()!=2)
            {
                qWarning("SemSolver::IO::readParameters - ERROR : wrong number of inpu"\
                         "ts on degree line.");
                file->close();
                return false;
            }
#endif
            parameters.setDegree(values[1].toInt(&degree));
        }
        else if(values[0]=="TOLERANCE")
        {
#ifdef SEMDEBUG
            if(values.size()!=2)
            {
                qWarning("SemSolver::IO::readParameters - ERROR : wrong number of inpu"\
                         "ts on tolerance line.");
                file->close();
                return false;
            }
#endif
            parameters.setTolerance(values[1].toDouble(&tolerance));
        }
        else if(values[0]=="PENALITY")
        {
#ifdef SEMDEBUG
            if(values.size()!=2)
            {
                qWarning("SemSolver::IO::readParameters - ERROR : wrong number of inpu"\
                         "ts on tolerance line.");
                file->close();
                return false;
            }
#endif
            parameters.setPenality(values[1].toDouble(&penality));
        }
#ifdef SEMDEBUG
        else
        {
            qWarning("SemSolver::IO::readParameters - ERROR : unknown input line in fi"\
                     "le.");
            file->close();
            return false;
        };
#endif
        values = next_non_empty_line_values(input);
    }
    file->close();
    return (degree && tolerance && penality);
};


#endif // IO_PARAMETERS_HPP
