#include "newgeometrydialog.hpp"

#include <QHeaderView>
#include <QMessageBox>

#include "geometrytolerancedialog.hpp"
#include "../lib/semsolver-io/pslg.hpp"
#include "../lib/semsolver-io/subdomains.hpp"
#include "../lib/semsolver-io/geometry.hpp"
#include "../lib/semsolver-preprocessor/computepolygonationfrompslg.hpp"

NewGeometryDialog::NewGeometryDialog(QWidget *parent,
                                     const QStringList &existing_geometries)
                                         : QDialog(parent),
                                         geometries(existing_geometries)
{
    layout = new QVBoxLayout(this);
    central_widget = new QWidget(this);
    central_layout = new QHBoxLayout(central_widget);
    inputs_widget = new QWidget(central_widget);
    inputs_layout = new QVBoxLayout(inputs_widget);
    inputs_tabs = new QTabWidget(inputs_widget);
    vertices = new QTableWidget();
    vertices->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    vertices->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vertices->setAlternatingRowColors(true);
    vertices->setColumnCount(2);
    vertices->setRowCount(0);
    vertices->horizontalHeader()->setStretchLastSection(true);
    vertices->horizontalHeader()->setClickable(false);
    vertices->verticalHeader()->setDefaultSectionSize(22);
    vertices->setColumnWidth(0,150);
    vertices->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
    vertices->setHorizontalHeaderItem(1, new QTableWidgetItem("y"));
    vertices->setSelectionBehavior(QAbstractItemView::SelectRows);
    vertices->setSelectionMode(QAbstractItemView::SingleSelection);
    segments = new QTableWidget();
    segments->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    segments->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    segments->setAlternatingRowColors(true);
    segments->setColumnCount(2);
    segments->setRowCount(0);
    segments->horizontalHeader()->setStretchLastSection(true);
    segments->horizontalHeader()->setClickable(false);
    segments->verticalHeader()->setDefaultSectionSize(22);
    segments->setColumnWidth(0,150);
    segments->setHorizontalHeaderItem(0, new QTableWidgetItem("Source"));
    segments->setHorizontalHeaderItem(1, new QTableWidgetItem("Target"));
    segments->setSelectionBehavior(QAbstractItemView::SelectRows);
    segments->setSelectionMode(QAbstractItemView::SingleSelection);
    holes = new QTableWidget();
    holes->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    holes->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    holes->setAlternatingRowColors(true);
    holes->setColumnCount(2);
    holes->setRowCount(0);
    holes->horizontalHeader()->setStretchLastSection(true);
    holes->horizontalHeader()->setClickable(false);
    holes->verticalHeader()->setDefaultSectionSize(22);
    holes->setColumnWidth(0,150);
    holes->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
    holes->setHorizontalHeaderItem(1, new QTableWidgetItem("y"));
    holes->setSelectionBehavior(QAbstractItemView::SelectRows);
    holes->setSelectionMode(QAbstractItemView::SingleSelection);
    inputs_tabs->addTab(vertices,"&Vertices");
    inputs_tabs->addTab(segments,"&Segments");
    inputs_tabs->addTab(holes,"&Holes");
    inputs_tabs->setFixedWidth(320);
    input = new QWidget(inputs_widget);
    input_layout = new QHBoxLayout(input);
    label0 = new QLabel(input);
    value0 = new QLineEdit(input);
    label1 = new QLabel(input);
    value1 = new QLineEdit(input);
    add_mod = new QPushButton(input);
    del = new QPushButton(input);
    label0->setText("x:");
    label1->setText("y:");
    add_mod->setText("+");
    add_mod->setFixedSize(24,24);
    add_mod->setEnabled(false);
    del->setText("-");
    del->setFixedSize(24,24);
    del->setEnabled(false);
    input_layout->addWidget(label0);
    input_layout->addWidget(value0);
    input_layout->addWidget(label1);
    input_layout->addWidget(value1);
    input_layout->addWidget(add_mod);
    input_layout->addWidget(del);
    input->setLayout(input_layout);
    input->setFixedWidth(320);
    message = new QLabel(inputs_widget);
    message->setAlignment(Qt::AlignRight);
    message->setFixedWidth(320);
    message->setText("");
    inputs_layout->addWidget(inputs_tabs);
    inputs_layout->addWidget(input);
    inputs_layout->addWidget(message);
    inputs_widget->setLayout(inputs_layout);
    previewer = new Viewer2D(central_widget);
    central_layout->addWidget(inputs_widget);
    central_layout->addWidget(previewer);
    central_widget->setLayout(central_layout);
    bottom_widget = new QWidget(this);
    bottom_layout = new QHBoxLayout(bottom_widget);
    label = new QLabel(bottom_widget);
    line_name = new QLineEdit(bottom_widget);
    cancel = new QPushButton(bottom_widget);
    save = new QPushButton(bottom_widget);
    label->setText("Name:");
    cancel->setText("&Cancel");
    cancel->setFixedSize(60,30);
    save->setText("&Save");
    save->setFixedSize(60,30);
    save->setEnabled(false);
    bottom_layout->addWidget(label);
    bottom_layout->addWidget(line_name);
    bottom_layout->addWidget(cancel);
    bottom_layout->addWidget(save);
    bottom_widget->setLayout(bottom_layout);
    layout->addWidget(central_widget);
    layout->addWidget(bottom_widget);
    this->setLayout(layout);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("New Geometry...");

    connect(inputs_tabs, SIGNAL(currentChanged(int)), this, SLOT(changeTab()));
    connect(value0, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(value1, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(add_mod, SIGNAL(clicked()), this, SLOT(addModifyInput()));
    connect(del, SIGNAL(clicked()), this, SLOT(deleteInput()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(save, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(vertices, SIGNAL(itemSelectionChanged()), this, SLOT(selectVerticesRow()));
    connect(segments, SIGNAL(itemSelectionChanged()), this, SLOT(selectSegmentsRow()));
    connect(holes, SIGNAL(itemSelectionChanged()), this, SLOT(selectHolesRow()));
    connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));

    selected_vertex = 0;
    selected_segment = 0;
    selected_hole = 0;
    value0->setFocus();

    GeometryToleranceDialog tolerance(this);
    if(!tolerance.exec())
        close();
    tol=tolerance.value();
    vertices_map = new SemSolver::PointsBimap<2, double>(tol);
    segments_map = new SemSolver::SegmentsMap<2, double>;
    holes_map = new SemSolver::PointsBimap<2, double>(tol);
};

NewGeometryDialog::~NewGeometryDialog()
{
    delete vertices_map;
    delete segments_map;
    delete holes_map;
    delete layout;
    delete central_layout;
    delete inputs_layout;
    vertices->clear();
    delete vertices;
    segments->clear();
    //delete segments;
    holes->clear();
    //delete holes;
    delete inputs_tabs;
    delete input_layout;
    delete label0;
    delete value0;
    delete label1;
    delete value1;
    delete add_mod;
    delete del;
    delete input;
    delete inputs_widget;
    delete previewer;
    delete central_widget;
    delete bottom_layout;
    delete label;
    delete line_name;
    delete cancel;
    delete save;
    delete bottom_widget;
};

void NewGeometryDialog::addVertex()
{
    double x = value0->text().toDouble();
    double y = value1->text().toDouble();
    int id = vertices_map->insertPoint(SemSolver::Point<2, double>(x, y));
    vertices->insertRow(id-1);
    vertices->setItem(id-1, 0, new QTableWidgetItem(QString::number(x)));
    vertices->setItem(id-1, 1, new QTableWidgetItem(QString::number(y)));
    vertices->item(id-1, 0)->setFlags(Qt::ItemIsSelectable);
    vertices->item(id-1, 1)->setFlags(Qt::ItemIsSelectable);
    previewer->addVertex(id, x, y);
    value0->setText("");
    value1->setText("");
    add_mod->setEnabled(false);
    value0->setFocus();
};

void NewGeometryDialog::modifyVertex()
{
    double x = value0->text().toDouble();
    double y = value1->text().toDouble();
    SemSolver::Point<2, double> new_vertex(x,y);
    SemSolver::Point<2, double> current_vertex = vertices_map->point(selected_vertex);
    vertices_map->modifyPoint(selected_vertex, new_vertex);
    vertices->item(selected_vertex-1,0)->setText(QString::number(x));
    vertices->item(selected_vertex-1,1)->setText(QString::number(y));
    previewer->modifyVertex(selected_vertex, x, y);
    SemSolver::Segment<2, double> segment_from(segments_map->segmentFrom(current_vertex));
    SemSolver::Segment<2, double> segment_to(segments_map->segmentTo(current_vertex));
    if(!segment_from.is_degenerate())
    {
        int segment_from_id = segments_map->id(segment_from);
        SemSolver::Point<2, double> const &target = segment_from.target();
        int target_id = vertices_map->id(target);
        segments_map->modify(segment_from_id,
                            SemSolver::Segment<2, double>(new_vertex, target));
        previewer->modifySegment(segment_from_id, vertices_map->point(selected_vertex),
                                 vertices_map->point(target_id));
    }
    if(!segment_to.is_degenerate())
    {
        int segment_to_id = segments_map->id(segment_to);
        SemSolver::Point<2,double> const &source = segment_to.source();
        int source_id = vertices_map->id(source);
        segments_map->modify(segment_to_id,
                            SemSolver::Segment<2,double>(source, new_vertex));
        previewer->modifySegment(segment_to_id, vertices_map->point(source_id),
                                 vertices_map->point(selected_vertex));
    }
    vertices->clearSelection();
    add_mod->setText("+");
    add_mod->setEnabled(false);
    del->setEnabled(false);
    value0->setText("");
    value1->setText("");
    value0->setFocus();
    selected_vertex = 0;
};

void NewGeometryDialog::deleteVertex()
{
    vertices_map->eraseId(selected_vertex);
    vertices->hideRow(selected_vertex-1);
    previewer->removeVertex(selected_vertex);
    vertices->clearSelection();
    add_mod->setText("+");
    add_mod->setEnabled(false);
    del->setEnabled(false);
    value0->setText("");
    value1->setText("");
    value0->setFocus();
    selected_vertex = 0;
};

void NewGeometryDialog::addSegment()
{
    int source_id = value0->text().toInt();
    int target_id = value1->text().toInt();
    SemSolver::Point<2, double> source = vertices_map->point(source_id);
    SemSolver::Point<2, double> target = vertices_map->point(target_id);
    SemSolver::Segment<2, double> segment(source, target);
    int id = segments_map->add(segment);
    segments->insertRow(id-1);
    segments->setItem(id-1,0,new QTableWidgetItem(QString::number(source_id)));
    segments->item(id-1,0)->setFlags(Qt::ItemIsSelectable);
    segments->setItem(id-1,1,new QTableWidgetItem(QString::number(target_id)));
    segments->
            item(id-1,1)->setFlags(Qt::ItemIsSelectable);
    previewer->addSegment(id, vertices_map->point(source_id),
                          vertices_map->point(target_id));
    segments->clearSelection();
    add_mod->setEnabled(false);
    value0->setText("");
    value1->setText("");
    value0->setFocus();
    selected_segment = 0;
};

void NewGeometryDialog::modifySegment()
{
    int source_id = value0->text().toInt();
    int target_id = value1->text().toInt();
    SemSolver::Point<2, double> source = vertices_map->point(source_id);
    SemSolver::Point<2, double> target = vertices_map->point(target_id);
    SemSolver::Segment<2, double> segment(source, target);
    segments_map->modify(selected_segment, segment);
    previewer->modifySegment(selected_segment, vertices_map->point(source_id),
                             vertices_map->point(target_id));
    segments->item(selected_segment-1, 0)->setText(QString::number(source_id));
    segments->item(selected_segment-1, 1)->setText(QString::number(target_id));
    segments->clearSelection();
    add_mod->setText("+");
    add_mod->setEnabled(false);
    del->setEnabled(false);
    value0->setText("");
    value1->setText("");
    value0->setFocus();
    selected_segment = 0;
};

void NewGeometryDialog::deleteSegment()
{
    segments_map->remove(selected_segment);
    segments->hideRow(selected_vertex-1);
    previewer->removeVertex(selected_vertex);
    segments->clearSelection();
    add_mod->setText("+");
    add_mod->setEnabled(false);
    del->setEnabled(false);
    value0->setText("");
    value1->setText("");
    value0->setFocus();
    selected_segment = 0;
};

void NewGeometryDialog::addHole()
{
    double x = value0->text().toDouble();
    double y = value1->text().toDouble();
    int id = holes_map->insertPoint(SemSolver::Point<2, double>(x, y));
    holes->insertRow(id-1);
    holes->setItem(id-1, 0, new QTableWidgetItem(QString::number(x)));
    holes->setItem(id-1, 1, new QTableWidgetItem(QString::number(y)));
    holes->item(id-1, 0)->setFlags(Qt::ItemIsSelectable);
    holes->item(id-1, 1)->setFlags(Qt::ItemIsSelectable);
    previewer->addHole(id, x, y);
    holes->clearSelection();
    add_mod->setEnabled(false);
    value0->setText("");
    value1->setText("");
    value0->setFocus();
    selected_hole= 0;
};

void NewGeometryDialog::modifyHole()
{
    double x = value0->text().toDouble();
    double y = value1->text().toDouble();
    holes_map->modifyPoint(selected_hole, SemSolver::Point<2, double>(x, y));
    holes->item(selected_hole-1, 0)->setText(QString::number(x));
    holes->item(selected_hole-1, 1)->setText(QString::number(y));
    previewer->modifyHole(selected_hole, x, y);
    holes->clearSelection();
    add_mod->setText("+");
    add_mod->setEnabled(false);
    del->setEnabled(false);
    value0->setText("");
    value1->setText("");
    value0->setFocus();
    selected_hole = 0;
};

void NewGeometryDialog::deleteHole()
{
    holes_map->eraseId(selected_hole);
    holes->hideRow(selected_hole-1);
    holes->clearSelection();
    add_mod->setText("+");
    add_mod->setEnabled(false);
    del->setEnabled(false);
    value0->setText("");
    value1->setText("");
    value0->setFocus();
    selected_hole = 0;
};

void NewGeometryDialog::checkName()
{
    QString string_name = line_name->text();
    if(string_name.isEmpty())
    {
        message->setText("");
        save->setEnabled(false);
    }
    else if(geometries.contains(string_name))
    {
        message->setText("existing file will be overwritten");
        save->setEnabled(true);
    }
    else
    {
        message->setText("");
        save->setEnabled(true);
    }
};

bool NewGeometryDialog::writePSLG(QFile *poly_file)
{
    if(poly_file->open(QIODevice::WriteOnly))
    {
        QTextStream output(poly_file);
        output << vertices_map->size() << "\t";
        output << 2 << "\t";
        output << 0 << "\t";
        output << 0 << "\n";
        for(SemSolver::PointsBimap<2, double>::ConstIterator it=vertices_map->begin();
        it!=vertices_map->end();
        ++it)
        {
            output << it->second <<"\t";
            output << it->first.x() << "\t";
            output << it->first.y() << "\n";
        }
        output << segments_map->segments() << "\t";
        output << 0 << "\n";
        for(SemSolver::SegmentsMap<2, double>::ConstIterator it=segments_map->begin();
        it!=segments_map->end(); ++it)
        {
            output << it->first << "\t";
            output << vertices_map->id(it->second.source()) << "\t";
            output << vertices_map->id(it->second.target()) << "\n";
        }
        output << holes_map->size() << "\n";
        for(SemSolver::PointsBimap<2, double>::ConstIterator it=holes_map->begin();
        it!=holes_map->end(); ++it)
        {
            output << it->second << "\t";
            output << it->first.x() << "\t";
            output << it->first.y() << "\n";
        }
        poly_file->close();
        return true;
    }
    return false;
};

void NewGeometryDialog::checkInput()
{
    // If input is uncomplete disable the add/modify button and show no message
    if(value0->text().isEmpty() || value1->text().isEmpty() )
    {
        add_mod->setEnabled(false);
        message->setText("");
        return;
    }
    int index = inputs_tabs->currentIndex();
    // If inputing a vertex
    if(index==0)
    {
        // check if input represents a point
        bool x_is_double, y_is_double;
        double x = value0->text().toDouble(&x_is_double);
        double y = value1->text().toDouble(&y_is_double);
        if (!x_is_double || !y_is_double)
        {
            add_mod->setEnabled(false);
            message->setText("values must be real");
            return;
        }
        SemSolver::Point<2,double> vertex(x, y);
        if(!selected_vertex)
        {
            // - check if point already exists
            if(vertices_map->hasPoint(vertex) || holes_map->hasPoint(vertex))
            {
                add_mod->setEnabled(false);
                message->setText("point already exists");
                return;
            }
            if(segments_map->haveOn(vertex))
            {
                add_mod->setEnabled(false);
                message->setText("point must not lie on a segment");
                return;
            }
        }
        else
        {
            SemSolver::Point<2,double> current_vertex = vertices_map->point(selected_vertex);
            // - check if point already exists
            if((vertices_map->hasPoint(vertex) && vertex!=current_vertex) || holes_map->hasPoint(vertex) )
            {
                add_mod->setEnabled(false);
                message->setText("point already exists");
                return;
            }
            SemSolver::Segment<2,double> segment_from = segments_map->segmentFrom(current_vertex);
            SemSolver::Segment<2,double> segment_to = segments_map->segmentTo(current_vertex);
            if( segments_map->haveOn(vertex) && !segment_from.has_on(vertex) && !segment_to.has_on(vertex) )
            {
                add_mod->setEnabled(false);
                message->setText("point must not lie on a segment");
                return;
            }
            if(!segment_from.is_degenerate() || !segment_to.is_degenerate())
            {
                del->setEnabled(false);
            }
        }
    }
    // If inputing a segment
    else if(index==1)
    {
        // - check if input represents a segment
        bool source_is_int, target_is_int;
        int source = value0->text().toInt(&source_is_int);
        int target = value1->text().toInt(&target_is_int);
        if (!source_is_int || !target_is_int)
        {
            add_mod->setEnabled(false);
            message->setText("values must be integer");
            return;
        }
        // - check if input is not degenerate
        if(source==target)
        {
            add_mod->setEnabled(false);
            message->setText("source and target must be different");
            return;
        }
        // - check if corresponding source and target exist
        if(!vertices_map->hasId(source) || !vertices_map->hasId(target))
        {
            add_mod->setEnabled(false);
            message->setText("source and target must exist");
            return;
        }
        SemSolver::Segment<2,double> segment(vertices_map->point(source), vertices_map->point(target));
        if(!selected_segment)
        {
            // - check if input already exist
            if(segments_map->contains(segment) || segments_map->contains(segment.opposite()))
            {
                add_mod->setEnabled(false);
                message->setText("segment already exists");
                return;
            }
            // - check if input is consistent
            if(!segments_map->isConsistentWith(segment))
            {
                add_mod->setEnabled(false);
                message->setText("segment must have consistent orientation");
                return;
            }
            // - check if input intersect existing segments
            if(segments_map->intersectInteriorly(segment))
            {
                add_mod->setEnabled(false);
                message->setText("segments must have have no intersection but edges");
                return;
            }
        }
        else
        {
            SemSolver::Segment<2,double> current_segment = segments_map->segment(selected_segment);
            segments_map->remove(selected_segment);
            if(segments_map->contains(segment) || \
               segments_map->contains(segment.opposite()))
            {
                add_mod->setEnabled(true);
                message->setText("segment already exists");
                segments_map->insert(selected_segment, current_segment);
                return;
            }
            // - check if input is consistent
            if(!segments_map->isConsistentWith(segment))
            {
                add_mod->setEnabled(false);
                message->setText("segment must have consistent orientation");
                segments_map->insert(selected_segment, current_segment);
                return;
            }
            // - check if input intersect existing segments
            if(segments_map->intersect(segment))
            {
                add_mod->setEnabled(false);
                message->setText("segments must have have no intersection but edges");
                segments_map->insert(selected_segment, current_segment);
                return;
            }
            segments_map->insert(selected_segment, current_segment);
        }
        if(vertices_map->hasPointOn(segment))
        {
            add_mod->setEnabled(false);
            message->setText("segments must not pass through vertices");
            return;
        }
        if(holes_map->hasPointOn(segment))
        {
            add_mod->setEnabled(false);
            message->setText("segments must not pass through holes");
            return;
        }
    }
    // If inputing an hole
    else
    {
        // - check if input represents a point
        // check if input represents a point
        bool x_is_double, y_is_double;
        double x = value0->text().toDouble(&x_is_double);
        double y = value1->text().toDouble(&y_is_double);
        if (!x_is_double || !y_is_double)
        {
            add_mod->setEnabled(false);
            message->setText("values must be real");
            return;
        }
        SemSolver::Point<2,double> hole(x, y);
        // - check if point already exists
        if(!selected_hole)
        {
            // - check if point already exists
            if(vertices_map->hasPoint(hole) || holes_map->hasPoint(hole))
            {
                add_mod->setEnabled(false);
                message->setText("point already exists");
                return;
            }
        }
        else
        {
            SemSolver::Point<2,double> current_hole = holes_map->point(selected_hole);
            // - check if point already exists
            if(vertices_map->hasPoint(hole) || (holes_map->hasPoint(hole) && hole!=current_hole) )
            {
                add_mod->setEnabled(false);
                message->setText("point already exists");
                return;
            }
        }
        if(segments_map->haveOn(hole))
        {
            add_mod->setEnabled(false);
            message->setText("point must not lie on a segment");
            return;
        }
    }
    // if all checks are succesfully passed enable the add button
    add_mod->setEnabled(true);
    message->setText("");
    return;
};

void NewGeometryDialog::addModifyInput()
{
    switch (inputs_tabs->currentIndex())
    {
    case 0:
        if(!selected_vertex)
            addVertex();
        else
            modifyVertex();
        break;
    case 1:
        if(!selected_segment)
            addSegment();
        else
            modifySegment();
        break;
    case 2:
        if(!selected_hole)
            addHole();
        else
            modifyHole();
        break;
    }
};

void NewGeometryDialog::deleteInput()
{
    switch (inputs_tabs->currentIndex())
    {
    case 0:
        deleteVertex();
        break;
    case 1:
        deleteSegment();
        break;
    case 2:
        deleteHole();
        break;
    }
};

void NewGeometryDialog::saveFile()
{
    QTemporaryFile temp_pslg_file;
    QTemporaryFile temp_subdomains_file;
    writePSLG(&temp_pslg_file);
    SemSolver::PSLG<double> pslg;
    SemSolver::Polygonation<2, double> polygonation;
    if(!SemSolver::IO::read_PSLG(&temp_pslg_file, pslg))
    {
        QMessageBox message;
        message.setText("Error. Temporary PSLG file corrupted.");
        message.exec();
        return;
    }
    if(!SemSolver::PreProcessor::compute_polygonation_from_pslg(pslg, polygonation))
    {
        QMessageBox message;
        message.setText("Error. The PSLG does not describe a valid polygon with holes.");
        message.exec();
        return;
    }
    if(!polygonation.isQuadrangulation())
        polygonation.refine();
    SemSolver::IO::write_subdomains(polygonation, &temp_subdomains_file);
    SemSolver::IO::write_geometry(&temp_pslg_file, &temp_subdomains_file, &temp_file);
    done(true);
};

void NewGeometryDialog::selectVerticesRow()
{
    int row = vertices->currentRow();
    selected_vertex = row+1;
    QString value0 = vertices->item(row,0)->text();
    QString value1 = vertices->item(row,1)->text();
    // Set the input to the modify mode
    add_mod->setText(QChar(0x2217));
    add_mod->setEnabled(true);
    del->setEnabled(true);
    // Set input values to the corresponding ones in the table
    this->value0->setText(value0);
    this->value1->setText(value1);
};

void NewGeometryDialog::selectSegmentsRow()
{
    int row = segments->currentRow();
    selected_segment = row+1;
    QString value0 = segments->item(row,0)->text();
    QString value1 = segments->item(row,1)->text();
    // Set the input to the modify mode
    add_mod->setText(QChar(0x2217));
    add_mod->setEnabled(true);
    del->setEnabled(true);
    // Set input values to the corresponding ones in the table
    this->value0->setText(value0);
    this->value1->setText(value1);
};

void NewGeometryDialog::selectHolesRow()
{
    int row = holes->currentRow();
    selected_hole = row+1;
    QString value0 = holes->item(row,0)->text();
    QString value1 = holes->item(row,1)->text();
    // Set the input to the modify mode
    add_mod->setText(QChar(0x2217));
    add_mod->setEnabled(true);
    del->setEnabled(true);
    // Set input values to the corresponding ones in the table
    this->value0->setText(value0);
    this->value1->setText(value1);
};

void NewGeometryDialog::changeTab()
{
    selected_vertex = 0;
    selected_segment = 0;
    selected_hole = 0;
    vertices->clearSelection();
    segments->clearSelection();
    holes->clearSelection();
    if (inputs_tabs->currentIndex()==1)
    {
        label0->setText("Source:");
        label1->setText("Target:");
    }
    else
    {
        label0->setText("x:");
        label1->setText("y:");
    }
    value0->setText("");
    value1->setText("");
    add_mod->setText("+");
    add_mod->setEnabled(false);
    del->setEnabled(false);
};
