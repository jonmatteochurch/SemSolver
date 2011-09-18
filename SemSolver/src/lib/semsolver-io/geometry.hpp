#ifndef IO_GEOMETRY_HPP
#define IO_GEOMETRY_HPP

#include <QFile>

#include <SemSolver/semgeometry.hpp>

#include <SemSolver/IO/archive.hpp>
#include <SemSolver/IO/pslg.hpp>
#include <SemSolver/IO/subdomains.hpp>

namespace SemSolver
{
    //! Namespace for Input/Output operations on SemSolver Classes
    namespace IO
    {
        //! Read SemGeometry form file
        template<class X>
        bool read_geometry(QFile *file,
                          SemGeometry<2, X> &geometry)
{
    Archive archive(file);
    if(!archive.openRead())
        return false;
    QTemporaryFile *poly_file, *domains_file;
    poly_file = new QTemporaryFile();
    domains_file = new QTemporaryFile();
    if(!archive.extractFile("pslg.poly", poly_file))
    {
        delete poly_file;
        delete domains_file;
        return false;
    }
    if(!archive.extractFile("domains.semsub", domains_file))
    {
        delete poly_file;
        delete domains_file;
        return false;
    }
    if(!archive.closeRead())
    {
        delete poly_file;
        delete domains_file;
        return false;
    }
    PSLG<double> pslg;
    if(!SemSolver::IO::read_PSLG(poly_file, pslg))
    {
        delete poly_file;
        delete domains_file;
        return false;
    }
    Polygonation<2,double> sub_domains;
    if(!SemSolver::IO::read_subdomains(domains_file, sub_domains))
    {
        delete poly_file;
        delete domains_file;
        return false;
    }
    geometry.setDomain(pslg);
    geometry.setSubDomains(sub_domains);
    delete poly_file;
    delete domains_file;
    return true;
};

        //! Write Geometry archive from pslg and subdomains files
        bool write_geometry(QFile *pslg_file,
                           QFile *domains_file,
                           QFile *file);
    };
};

#endif // IO_GEOMETRY_HPP
