#include "archive.hpp"

#include <QFileInfo>

SemSolver::IO::Archive::Archive(QFile *qfile)
    : file(qfile)
{
    status = CLOSED;
}

bool SemSolver::IO::Archive::openRead()
{
#ifdef SEMDEBUG
    if(status != CLOSED)
        qFatal("Archive must be closed before openread");
    if(!file->exists())
        return false;
#endif
    QFileInfo info(*file);
    archive = archive::archive_read_new();
    archive::archive_read_support_compression_none(archive);
    archive::archive_read_support_format_all(archive);
    if(archive::archive_read_open_filename(archive,
                                           info.canonicalFilePath().toLatin1(),
                                           ARCHIVE_DEFAULT_BYTES_PER_BLOCK)
        != ARCHIVE_OK)
        return false;
    status = OPENREAD;
    return true;
};

QStringList SemSolver::IO::Archive::entries()
{
#ifdef SEMDEBUG
    if(status != OPENREAD)
        qFatal("You must openRead archive before reading entries");
#endif
    closeRead();
    openRead();
    QStringList list;
    archive::archive_entry *entry;
    while (archive::archive_read_next_header(archive, &entry) == ARCHIVE_OK)
        list.push_back(archive::archive_entry_pathname(entry));
    return list;
}

bool SemSolver::IO::Archive::extractFile(const QString &name, QFile *file)
{
#ifdef SEMDEBUG
    if(status != OPENREAD)
        qFatal("You must openRead archive before extracting entries");
#endif
    closeRead();
    openRead();
    archive::archive_entry *entry;

    while (archive::archive_read_next_header(archive, &entry) == ARCHIVE_OK)
    {
        if(QString(archive::archive_entry_pathname(entry))==name)
        {
            if(!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
                return false;
            qint64 size = archive::archive_entry_size(entry);
            char *data = new char[size];
            if(size != archive::archive_read_data(archive, data,size))
            {
                delete[] data;
                return false;
            }
            if(size!=file->write(data, size))
            {
                delete[] data;
                return false;
            }
            delete[] data;
            file->close();
            return true;
        }
    }
    return false;
}

bool SemSolver::IO::Archive::closeRead()
{
#ifdef SEMDEBUG
    if(status != OPENREAD)
        qFatal("You must openRead archive before closing it");
#endif
    if (archive::archive_read_finish(archive) != ARCHIVE_OK)
    {
        return false;
    }
    status = CLOSED;
    return true;
}

bool SemSolver::IO::Archive::closeWrite()
{
#ifdef SEMDEBUG
    if(status != OPENWRITE)
        qFatal("You must openWrite archive before closing it");
#endif
    if (archive::archive_write_finish(archive) != ARCHIVE_OK)
        return false;
    status = CLOSED;
    return true;
}

bool SemSolver::IO::Archive::openWrite()
{
#ifdef SEMDEBUG
    if(status != CLOSED)
        qFatal("archive is not closed while attempting to open it in write mode");
    if(!file->exists())
    {
        if (!file->open(QIODevice::WriteOnly))
            return false;
        file->close();
    }
#else
    if(!file->exists())
    {
        file->open(QIODevice::WriteOnly);
        file->close();
    }
#endif
    QFileInfo info(*file);
    archive = archive::archive_write_new();
    archive::archive_write_set_compression_none(archive);
    archive::archive_write_set_format_pax_restricted(archive);
    if(archive::archive_write_open_filename(archive,
                                            info.canonicalFilePath().toLatin1())
        != ARCHIVE_OK)
        return false;
    status = OPENWRITE;
    return true;
}

bool SemSolver::IO::Archive::addFile(QFile *file,
                                     const QString &name)
{
#ifdef SEMDEBUG
    if(status != OPENWRITE)
        qFatal("You must openWrtie archive before adding entries");
    if(!file->exists())
        return false;
#endif
    qint64 size = file->size();
    archive::archive_entry *entry = archive::archive_entry_new();
    archive::archive_entry_set_pathname(entry, name.toLatin1());
    archive::archive_entry_set_size(entry, size);
    archive::archive_entry_set_filetype(entry, AE_IFREG);
    archive::archive_entry_set_perm(entry, 0644);
    archive::archive_write_header(archive, entry);
    if(!file->open(QIODevice::ReadOnly))
        return false;
    QByteArray data = file->readAll();
    file->close();
    archive::archive_write_data(archive, data.constData(), size);
    archive::archive_entry_free(entry);
    return true;
};
