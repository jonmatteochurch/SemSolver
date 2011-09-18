#ifndef NEXTNONEMPTLINEVALUES_HPP
#define NEXTNONEMPTLINEVALUES_HPP

#include <QStringList>
#include <QTextStream>

namespace SemSolver
{
    //! Namespace for Input/Output operations on SemSolver Classes
    namespace IO
    {
        //! Get list of values on next non empty line in a text stream skipping comments
        QStringList next_non_empty_line_values(QTextStream &text_stream);
    };
};

#endif // NEXTNONEMPTLINEVALUES_HPP
