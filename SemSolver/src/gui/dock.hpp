#ifndef DOCK_HPP
#define DOCK_HPP

#include <QComboBox>
#include <QDockWidget>
#include <QFileInfo>
#include <QGroupBox>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>

class Dock
    : public QDockWidget
{
    Q_OBJECT

    // interface
    QWidget     *widget;
    QVBoxLayout *layout;
    QGroupBox   *workspace;
    QVBoxLayout *workspace_layout;
    QLabel      *workspace_label;
    QGroupBox   *geometry;
    QVBoxLayout *geometry_layout;
    QGroupBox   *equation;
    QVBoxLayout *equation_layout;
    QGroupBox   *boundary_conditions;
    QVBoxLayout *boundary_conditions_layout;
    QGroupBox   *parameters;
    QVBoxLayout *parameters_layout;

public:
    QComboBox   *geometry_list;
    QComboBox   *equation_list;
    QComboBox   *boundary_conditions_list;
    QComboBox   *parameters_list;

    Dock(QWidget *parent = 0);
    ~Dock();

    void setWorkspace(QFileInfo const &workspace);
    void setGeometries(QStringList const &geometries);
    void setEquations(QStringList const &equations);
    void setBoundaryConditions(QStringList const &boundary_conditions);
    void setParameters(QStringList const &parameters);

    void resetWorkspace();

    void selectGeometry(QString const &geometry);
    void selectEquation(QString const &equation);
    void selectBoundaryConditions(QString const &boundary_conditions);
    void selectParameters(QString const &parameters);

public slots:
    void refresh(bool is_floating);
};

#endif // DOCK_HPP
