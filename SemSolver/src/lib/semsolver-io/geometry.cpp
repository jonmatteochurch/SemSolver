#include "geometry.hpp"

#include "../semsolver-io/archive.hpp"

bool SemSolver::IO::write_geometry(QFile *pslg_file,
                                  QFile *subdomains_file,
                                  QFile *file)
{
    Archive archive(file);
    archive.openWrite();
    archive.addFile(pslg_file, "pslg.poly");
    archive.addFile(subdomains_file, "domains.semsub");
    archive.closeWrite();
    return true;
}
