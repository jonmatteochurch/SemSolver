#ifndef IO_PSLG_HPP
#define IO_PSLG_HPP

#include <QFile>

#include <SemSolver/pslg.hpp>
#include <SemSolver/IO/nextnonemptlinevalues.hpp>

namespace SemSolver
{
    //! Namespace for Input/Output operations on SemSolver Classes
    namespace IO
    {
        template<class X>
        bool read_PSLG(QFile *file,
                      PSLG<X> &pslg)
{
    pslg.clear();

#ifdef SEMDEBUG
    bool is_open =
#endif
            file->open(QIODevice::ReadOnly);
#ifdef SEMDEBUG
    if(!is_open)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : cannot open 'poly_file'");
        return false;
    }
#endif
    QTextStream input(file);
    QStringList values;
    values = next_non_empty_line_values(input);
#ifdef SEMDEBUG
    if(values.isEmpty())
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : missing 'number_of_vertices'");
        file->close();
        return false;
    }
#endif
    bool ok;
    int number_of_vertices = values[0].toInt(&ok);
#ifdef SEMDEBUG
    if(!ok)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_vertices' must be an inte"\
                 "ger");
        file->close();
        return false;
    }
    if(number_of_vertices < 0)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_vertices' must be non-neg"\
                 "ative");
        file->close();
        return false;
    }
#endif
    pslg.setNumberOfVertices(number_of_vertices);
#ifdef SEMDEBUG
    if(values.size()<2)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : missing 'dimension'");
        file->close();
        return false;
    }
    int dimension = values[1].toInt(&ok);
    if(!ok)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'dimension' must be an integer");
        file->close();
        return false;
    }
    if(dimension != 2)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'dimension' must be 2");
        file->close();
        return false;
    }
    if(values.size()<3)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : missing 'number_of_attributes'");
        file->close();
        return false;
    }
    int number_of_vertices_attributes = values[2].toInt(&ok);
    if(!ok)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_vertices_attributes' must"\
                 " be an integer");
        file->close();
        return false;
    }
    if(number_of_vertices_attributes < 0)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_vertices_attributes' must"\
                 " be non-negative");
        file->close();
        return false;
    }
    if(number_of_vertices_attributes > 0)
        qWarning("SemSolver::IO::readPSLG - ERROR : ignoring vertices attributes in 'pol"\
                 "y_file'");
#endif
    pslg.setNumberOfVerticesAttributes(0);
#ifdef SEMDEBUG
    if(values.size()<4)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : missing 'number_of_vertices_boundary"\
                 "_markers'");
        file->close();
        return false;
    }
    int number_of_vertices_boundary_markers = values[3].toInt(&ok);
    if(!ok)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_vertices_boundary_markers"\
                 "' must be an integer");
        file->close();
        return false;
    }
    if(number_of_vertices_boundary_markers!=0 && number_of_vertices_boundary_markers != 1)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_vertices_boundary_markers"\
                 "' must be 0 or 1");
        file->close();
        return false;
    }
    if(number_of_vertices_boundary_markers > 0)
        qWarning("SemSolver::IO::readPSLG - ERROR : ignoring vertices boundary markers i"\
                 "\n 'poly_file'");
#endif
    pslg.setNumberOfVerticesBoundaryMarkers(0);
#ifdef SEMDEBUG
    if(values.size()>4)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : too many inputs on first line");
        file->close();
        return false;
    }
#endif
    for(int i=0; i<number_of_vertices; ++i)
    {
        values = next_non_empty_line_values(input);
#ifdef SEMDEBUG
        if(values.isEmpty())
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Vertex::vertex_number'");
            file->close();
            return false;
        }
#endif
        int vertex_number = values[0].toInt(&ok);
#ifdef SEMDEBUG
        if(!ok)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : 'Vertex::vertex_number' must be "\
                     "an integer");
            file->close();
            return false;
        }
        for(int j=0; j<i; ++j)
        {
            if(vertex_number==pslg.vertex(j).number)
            {
                qWarning("SemSolver::IO::readPSLG - ERROR : 'Vertex::vertex_number' must"\
                         "be unique");
                file->close();
                return false;
            }
        }
        if(values.size()<2)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Vertex::x'");
            file->close();
            return false;
        }
#endif
        double x = values[1].toDouble(&ok);
#ifdef SEMDEBUG
        if(!ok)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Vertex::x' must be a do"\
                     "uble");
            file->close();
            return false;
        }
        if(values.size()<3)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Vertex::y'");
            file->close();
            return false;
        }
#endif
        double y = values[2].toDouble(&ok);
#ifdef SEMDEBUG
        if(!ok)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : 'Vertex::y' must be a double");
            file->close();
            return false;
        }
        if(values.size()<3+number_of_vertices_attributes)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Vertex::attribute'");
            file->close();
            return false;
        }
        if(values.size()<3+number_of_vertices_attributes
           +number_of_vertices_boundary_markers)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Vertex::boundary_marker"\
                     "'");
            file->close();
            return false;
        }
#endif
        pslg.setVertex(i,vertex_number,x,y);
    }
    values = next_non_empty_line_values(input);
#ifdef SEMDEBUG
    if(values.isEmpty())
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : missing 'number_of_segments'");
        file->close();
        return false;
    }
#endif
    int number_of_segments = values[0].toInt(&ok);
#ifdef SEMDEBUG
    if(!ok)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_segments' must be an inte"\
                 "ger");
        file->close();
        return false;
    }
    if (number_of_segments < 0)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_segments' must be non-neg"\
                 "ative");
        file->close();
        return false;
    }
#endif
    pslg.setNumberOfSegments(number_of_segments);
#ifdef SEMDEBUG
    if(values.size()<2)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : missing 'number_of_segments_boundary"\
                 "_markers'");
        file->close();
        return false;
    }
    int number_of_segments_boundary_markers = values[1].toInt(&ok);
    if(!ok)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_segments_boundary_markers"\
                 "' must be an integer");
        file->close();
        return false;
    }
    if (number_of_segments_boundary_markers!=0 && number_of_segments_boundary_markers!=1)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_segments_boundary_markers"\
                 "' must be 0 or 1");
        file->close();
        return false;
    }
    if(number_of_segments_boundary_markers > 0)
        qWarning("SemSolver::IO::readPSLG - ERROR : ignoring segments boundary markers i"\
                 "n 'poly_file'");
    if(values.size()>2)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'too many inputs on segments header "\
                 "line");
        file->close();
        return false;
    }
#endif
    for(int i=0; i<number_of_segments; ++i)
    {
        values = next_non_empty_line_values(input);
#ifdef SEMDEBUG
        if(values.isEmpty())
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Segment::segment_number"\
                     "'");
            file->close();
            return false;
        }
#endif
        int segment_number = values[0].toInt(&ok);
#ifdef SEMDEBUG
        if(!ok)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : 'Segment::segment_number' must b"\
                     "e an integer");
            file->close();
            return false;
        }
        for(int j=0; j<i; ++j)
        {
            if(segment_number==pslg.segment(j).number)
            {
                qWarning("SemSolver::IO::readPSLG - ERROR : 'Segment::segment_number' mu"\
                         "st be unique");
                file->close();
                return false;
            }
        }
        if(values.size()<2)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Segment::source'");
            file->close();
            return false;
        }
#endif
        int source = values[1].toInt(&ok);
#ifdef SEMDEBUG
        if(!ok)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR  : 'Segment::source' must be an in"\
                     "teger");
            file->close();
            return false;
        }
        if(values.size()<3)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Segment::target'");
            file->close();
            return false;
        }
#endif
        int target = values[2].toInt(&ok);
#ifdef SEMDEBUG
        if(!ok)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : 'Segment::target' must be an int"\
                     "eger");
            file->close();
            return false;
        }
        bool found_source = false;
        bool found_target = false;
        for(int j=0; j<number_of_vertices && !(found_source&&found_target); ++j)
        {
            if(pslg.vertex(j).number==source)
                found_source = true;
            if(pslg.vertex(j).number==target)
                found_target = true;
        }
        if(!found_source)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : 'Segment::source' must be an exi"\
                     "sting 'vertex_number'");
            file->close();
            return false;
        }
        if(!found_target)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR  : 'Segment::target' must be an ex"\
                     "isting 'vertex_number'");
            file->close();
            return false;
        }
        if(values.size()<3+number_of_segments_boundary_markers)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR  : missing 'Segment::boundary_mark"\
                     "er'");
            file->close();
            return false;
        }
        if(values.size()>3+number_of_segments_boundary_markers)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : too many inputs on segment line");
            file->close();
            return false;
        }
#endif
        pslg.setSegment(i, segment_number, source, target);
    }
    values = next_non_empty_line_values(input);
#ifdef SEMDEBUG
    if(values.isEmpty())
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : missing 'number_of_holes'");
        file->close();
        return false;
    }
#endif
    int number_of_holes = values[0].toInt(&ok);
#ifdef SEMDEBUG
    if(!ok)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'Segment::number_of_holes' must be a"\
                 "n integer");
        file->close();
        return false;
    }
    if (number_of_holes < 0)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR : 'number_of_holes' must be non-negati"\
                 "ve");
        file->close();
        return false;
    }
    if(values.size()>1)
    {
        qWarning("SemSolver::IO::readPSLG - ERROR  : too many inputs on holes header lin"\
                 "e");
        file->close();
        return false;
    }
#endif
    pslg.setNumberOfHoles(number_of_holes);
    for(int i=0; i<number_of_holes; ++i)
    {
        values = next_non_empty_line_values(input);
#ifdef SEMDEBUG
        if(values.isEmpty())
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Hole::hole_number'");
            file->close();
            return false;
        }
#endif
        int hole_number = values[0].toInt(&ok);
#ifdef SEMDEBUG
        if(!ok)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : 'Hole::hole_number' must be an i"\
                     "nteger");
            file->close();
            return false;
        }
        for(int j=0; j<i; ++j)
        {
            if(hole_number==pslg.hole(j).number)
            {
                qWarning("SemSolver::IO::readPSLG - ERROR : 'Hole::hole_number' must be "\
                         "unique");
                file->close();
                return false;
            }
        }
        if(values.size()<2)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Hole::x'");
            file->close();
            return false;
        }
#endif
        double x = values[1].toDouble(&ok);
#ifdef SEMDEBUG
        if(!ok)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : 'Hole::x' must be a double");
            file->close();
            return false;
        }
        if(values.size()<3)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : missing 'Hole::y'");
            file->close();
            return false;
        }
#endif
        double y = values[2].toDouble(&ok);
#ifdef SEMDEBUG
        if(!ok)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : 'Hole::y' must be a double");
            file->close();
            return false;
        }
        if(values.size()>3)
        {
            qWarning("SemSolver::IO::readPSLG - ERROR : 'too many inputs on hole line");
            file->close();
            return false;
        }
#endif
        pslg.setHole(i, hole_number, x, y);
    }
    values = next_non_empty_line_values(input);
#ifdef SEMDEBUG
    if(!values.isEmpty())
        qWarning("SemSolver::IO::readPSLG - ERROR : ignoring extra information in 'poly_"\
                 "file'");
#endif
    file->close();
    return true;
};
    };
};

#endif // IO_PSLG_HPP
