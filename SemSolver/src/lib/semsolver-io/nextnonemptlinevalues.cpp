#include "nextnonemptlinevalues.hpp"

QStringList SemSolver::IO::next_non_empty_line_values(QTextStream &text_stream)
{
    QStringList line_values;
    do
    {
        if(text_stream.atEnd())
        {
            line_values.clear();
            return line_values;
        }
        QString line = text_stream.readLine();
        line = line.section('#',0,0);
        line_values = line.split(QRegExp("\\s+"),QString::SkipEmptyParts);
    }
    while(line_values.isEmpty());
    return line_values;
};
