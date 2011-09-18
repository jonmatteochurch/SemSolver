#ifndef IO_ARCHIVE_HPP
#define IO_ARCHIVE_HPP

#include <QFile>
#include <QString>
#include <QStringList>
#include <QTemporaryFile>
#include <QTextStream>

#include  <SemSolver/IO/carchive>

namespace SemSolver
{
    //! Namespace for Input/Output operations on SemSolver Classes
    namespace IO
    {
        //! Class for handling tar uncompressed archives
        class Archive
        {
            //! Archive status
            //! Can be closed or open in read or write mode
            enum Status
            {
                CLOSED,
                OPENREAD,
                OPENWRITE
            };

            archive::archive *archive;
            QFile            *file;
            Status           status;

        public:
            //! Default constructor
            //! file pointer to archive file
            Archive(QFile *file);

            //! Open archive in read mode
            bool openRead();

            //! Open archive in write mode
            bool openWrite();

            //! Close archive in read mode
            bool closeRead();

            //! Close archive in write mode
            bool closeWrite();

            //! Get list of entries in archive
            QStringList entries();

            //! Add a new entry to archive
            //! \param name Name of the entry
            //! \param value Value of the entry
            template<class T>
            bool addValue(T const &value,
                          QString const &name)
            {
            #if SEMDEBUG
                if(status!=OPENWRITE)
                    qFatal("You must openWrite archive before adding entries");
            #endif
                if(!file->exists())
                    return false;
                QTemporaryFile *temp_file = new QTemporaryFile();
                if(!temp_file->open(QIODevice::WriteOnly | QIODevice::Text))
                    return false;
                QTextStream out(temp_file);
                out << value;
                temp_file->close();
                addFile(temp_file, name);
                delete temp_file;
                return true;
            };

            //! Add a new entry to archive
            //! \param file File to be used as entry
            bool addFile(QFile *file);

            //! Add a new entry to archive
            //! \param name Name of the entry
            //! \param file File to be used as entry
            bool addFile(QFile *file,
                         QString const &name);

            //! Get an entry
            //! \param name Name of the entry
            bool extractFile(QString const &name);

            //! Get an entry
            //! \param file Pointer to the file where to extract entry data
            //! \param name Name of the entry
            bool extractFile(QString const &name, QFile *file);
        };
    };
};

#endif // IO_ARCHIVE_HPP
