#include "exportplotdialog.hpp"

#include <qwt3d_io.h>

ExportPlotDialog::ExportPlotDialog(QWidget *parent)
    : QFileDialog(parent)
{
    setFileMode(QFileDialog::AnyFile);
    QStringList list = Qwt3D::IO::outputFormatList();
    if(list.contains("bmp"))
    {
        filters.push_back("Bitmap (*.bmp)");
        formats.push_back("bmp");
    }
    if(list.contains("jpeg"))
    {
        filters.push_back("JPEG (*.jpeg *.jpg)");
        formats.push_back("jpg");
    }
    if(list.contains("png"))
    {
        filters.push_back("Portable Network Graphics (*.png)");
        formats.push_back("png");
    }
    if(list.contains("ppm"))
    {
        filters.push_back("Portable Pixmap (*.ppm)");
        formats.push_back("ppm");
    }
    if(list.contains("tif"))
    {
        filters.push_back("Tagged Image (*.tiff *.tif)");
        formats.push_back("tif");
    }
    if(list.contains("xbm"))
    {
        filters.push_back("X BitMap (*.xbm)");
        formats.push_back("xbm");
    }
    if(list.contains("xpm"))
    {
        filters.push_back("X PixMap (*.xpm)");
        formats.push_back("xpm");
    }
    if(list.contains("EPS"))
    {
        filters.push_back("Encapsulated PostScript (*.eps)");
        formats.push_back("EPS");
    }
    if(list.contains("PS"))
    {
        filters.push_back("PostScript (*.ps)");
        formats.push_back("PS");
    }
    if(list.contains("PDF"))
    {
        filters.push_back("Portable Document (*.pdf)");
        formats.push_back("PDF");
    }
    if(list.contains("SVG"))
    {
        filters.push_back("Scalable Vector Graphics (*.svg)");
        formats.push_back("SVG");
    }
    if(list.contains("PGF"))
    {
        filters.push_back("Portable Graphics (*.pgf)");
        formats.push_back("PGF");
    }

    setNameFilters(filters);
    setWindowTitle("Export Plot");
    setAcceptMode(QFileDialog::AcceptSave);
    setConfirmOverwrite(true);
};

