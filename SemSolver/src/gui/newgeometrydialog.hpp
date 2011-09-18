#ifndef NEWGEOMETRYDIALOG_HPP
#define NEWGEOMETRYDIALOG_HPP

#include <QDialog>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QTabWidget>
#include <QTemporaryFile>
#include <QVBoxLayout>
#include <QWidget>

#include "../lib/semsolver/pointsbimap.hpp"
#include "../lib/semsolver/segmentsmap.hpp"

#include "viewer2d.hpp"

class NewGeometryDialog : public QDialog
{
    Q_OBJECT

    // interface
    QVBoxLayout *layout;
    QWidget *central_widget;
    QHBoxLayout *central_layout;
    QWidget *inputs_widget;
    QVBoxLayout *inputs_layout;
    QTabWidget *inputs_tabs;
    QTableWidget *vertices;
    QTableWidget *segments;
    QTableWidget *holes;
    QLabel *message;
    QWidget *input;
    QHBoxLayout *input_layout;
    QLabel *label0;
    QLineEdit *value0;
    QLabel *label1;
    QLineEdit *value1;
    QPushButton *add_mod;
    QPushButton *del;
    Viewer2D *previewer;
    QWidget *bottom_widget;
    QHBoxLayout *bottom_layout;
    QLabel *label;
    QLineEdit *line_name;
    QPushButton *cancel;
    QPushButton *save;

    // variables
    QTemporaryFile temp_file;
    QStringList const &geometries;
    SemSolver::PointsBimap<2, double> *vertices_map;
    SemSolver::SegmentsMap<2, double> *segments_map;
    SemSolver::PointsBimap<2, double> *holes_map;
    int selected_vertex;
    int selected_segment;
    int selected_hole;

    double tol;

public:
    NewGeometryDialog(QWidget *parent,
                      QStringList const &existing_geometries);
    ~NewGeometryDialog();
    inline QTemporaryFile *file()
		{
    return &temp_file;
};
    inline QString name()
{
    return line_name->text();
};

private:
    void addVertex();
    void modifyVertex();
    void deleteVertex();

    void addSegment();
    void modifySegment();
    void deleteSegment();

    void addHole();
    void modifyHole();
    void deleteHole();

    bool writePSLG(QFile *poly_file);

private slots:
    void checkName();
    void checkInput();
    void addModifyInput();
    void deleteInput();
    void saveFile();
    void selectVerticesRow();
    void selectSegmentsRow();
    void selectHolesRow();
    void changeTab();
};

#endif // NEWGEOMETRYDIALOG_HPP
