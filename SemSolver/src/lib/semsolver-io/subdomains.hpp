#ifndef IO_SUBDOMAINS_HPP
#define IO_SUBDOMAINS_HPP

#include <QFile>
#include <QTextStream>
#ifdef SEMDEBUG
#  include <QDebug>
#endif

#include  <SemSolver/polygonation.hpp>

namespace SemSolver
{
    //! Namespace for Input/Output operations on SemSolver Classes
    namespace IO
    {
        //! Read subdomains Polygonation from file
        template<class X>
        bool read_subdomains(QFile *file,
                            Polygonation<2, X> &sub_domains)
{
    if(!file->open(QIODevice::ReadOnly))
        return false;
    QTextStream input(file);
    sub_domains.clear();
    int size, polygon_size, id, neighbour;
    double x, y;
    Polygon<2,X> polygon;
    std::vector<int> neighbours;
    input >> size;
    for(int i=0; i<size; ++i)
    {
        polygon.clear();
        neighbours.clear();
        input >> id;
#ifdef SEMDEBUG
        if(id!=i+1)
        {
            qWarning() << "corrupted domains file";
            return false;
        }
#endif
        input >> polygon_size;
        for(int j=0; j<polygon_size; ++j)
        {
            input >> x;
            input >> y;
            input >> neighbour;
            polygon.push_back(Point<2,X>(x,y));
            neighbours.push_back(neighbour);
        }
        sub_domains.addElement(polygon, neighbours);
    }
    file->close();
    return true;
};

        //! Write subdomains Polygonation to file
        template<class X>
        bool write_subdomains(Polygonation<2, X> const &sub_domains,
                             QFile *file)
{
    if(!file->open(QIODevice::WriteOnly))
        return false;
    QTextStream output(file);
    int n = sub_domains.size();
    output << n << "\n";
    for(int i=0; i<n; ++i)
    {
        int m = sub_domains.element(i).size();
        output << i+1 << "\t" << m << "\n";
        std::vector<int>::const_iterator it = sub_domains.element(i).neighboursBegin();
        for(int j=0; j<m; ++j)
        {
            output << sub_domains.element(i).vertex(j).x() << "\t";
            output << sub_domains.element(i).vertex(j).y() << "\t";
            output << *it++ << "\n";
        }
    }
    file->close();
    return true;
};
    };
};

#endif // IO_SUBDOMAINS_HPP
