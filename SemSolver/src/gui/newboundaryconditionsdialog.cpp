#include "newboundaryconditionsdialog.hpp"

#include <QHeaderView>
#include <QMessageBox>
#include <QScriptEngine>
#include <QScriptSyntaxCheckResult>

NewBoundaryConditionsDialog::NewBoundaryConditionsDialog(
	QWidget *parent, 
        const QStringList &existing_boundary_conditions)
            : QDialog(parent), boundary_conditions(existing_boundary_conditions)
{
    layout = new QVBoxLayout(this);
    table = new QTableWidget(this);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setAlternatingRowColors(true);
    table->setColumnCount(1);
    table->setRowCount(0);
    table->horizontalHeader()->hide();
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setClickable(false);
    table->verticalHeader()->setDefaultSectionSize(22);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    input_layout0 = new QHBoxLayout;
    input_layout1 = new QHBoxLayout;
    input_layout2 = new QHBoxLayout;
    border_label = new QLabel(this);
    border_value = new QLineEdit(this);
    condition_label = new QLabel(this);
    conditions_list = new QComboBox(this);
    condition = new Formula(this);
    label0 = new Formula(this);
    value0 = new QLineEdit(this);
    label1 = new Formula(this);
    value1 = new QLineEdit(this);
    add_mod = new QPushButton(this);
    del = new QPushButton(this);
    border_label->setText("<b>Border:</b>");
    border_value->setFixedWidth(60);
    condition_label->setText("<b>Condition</b>");
    conditions_list->addItem("Dirichlet");
    conditions_list->addItem("Neumann");
    conditions_list->addItem("Robin");
    conditions_list->setCurrentIndex(0);
    add_mod->setText("+");
    add_mod->setFixedSize(24,24);
    add_mod->setEnabled(false);
    del->setText("-");
    del->setFixedSize(24,24);
    del->setEnabled(false);
    input_layout0->addWidget(border_label);
    input_layout0->addWidget(border_value);
    input_layout0->addWidget(condition_label);
    input_layout0->addWidget(conditions_list);
    input_layout0->addStretch();
    input_layout1->addStretch();
    input_layout1->addWidget(condition);
    input_layout1->addStretch();
    input_layout2->addWidget(label0);
    input_layout2->addWidget(value0);
    input_layout2->addWidget(label1);
    input_layout2->addWidget(value1);
    input_layout2->addWidget(add_mod);
    input_layout2->addWidget(del);
    message = new QLabel(this);
    message->setAlignment(Qt::AlignRight);
    message->setText("");
    bottom_widget = new QWidget(this);
    bottom_layout = new QHBoxLayout(bottom_widget);
    label = new QLabel(bottom_widget);
    line_name = new QLineEdit(bottom_widget);
    cancel = new QPushButton(bottom_widget);
    button_save = new QPushButton(bottom_widget);
    label->setText("Name:");
    cancel->setText("&Cancel");
    cancel->setFixedSize(60,30);
    button_save->setText("&Save");
    button_save->setFixedSize(60,30);
    button_save->setEnabled(false);
    bottom_layout->addWidget(label);
    bottom_layout->addWidget(line_name);
    bottom_layout->addWidget(cancel);
    bottom_layout->addWidget(button_save);
    bottom_widget->setLayout(bottom_layout);
    layout->addWidget(table);
    layout->addLayout(input_layout0);
    layout->addSpacing(12);
    layout->addLayout(input_layout1);
    layout->addSpacing(12);
    layout->addLayout(input_layout2);
    layout->addWidget(message);
    layout->addWidget(bottom_widget);
    this->setLayout(layout);
    this->setBoundaryCondition(0);

    connect(conditions_list, SIGNAL(currentIndexChanged(int)), this,
            SLOT(setBoundaryCondition(int)));
    connect(border_value, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(value0, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(value1, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(add_mod, SIGNAL(clicked()), this, SLOT(addModifyInput()));
    connect(del, SIGNAL(clicked()), this, SLOT(deleteInput()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(selectRow()));
    connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));

    selected_row = 0;
};

NewBoundaryConditionsDialog::~NewBoundaryConditionsDialog()
{
    delete table;
    delete message;
    delete border_label;
    delete border_value;
    delete condition_label;
    delete conditions_list;
    delete condition;
    delete label0;
    delete value0;
    delete label1;
    delete value1;
    delete add_mod;
    delete del;
    delete label;
    delete line_name;
    delete cancel;
    delete button_save;
    delete input_layout0;
    delete input_layout1;
    delete input_layout2;
    delete bottom_layout;
    delete bottom_widget;
    delete layout;
};

void NewBoundaryConditionsDialog::setBoundaryCondition(const int &index)
{
    if(index==0)
    {
        condition->setContent("<mpadded depth='-2'><msub><mi>u</mi><mrow><mo>&#124;</mo>"\
                              "<msub><mi>&Gamma;</mi><mi>i</mi></msub></mrow></msub><mo>"\
                              "=</mo><msub><mi>g</mi><mi>i</mi></msub></mpadded>");
        condition->setFixedSize(condition->sizeHint());
        label0->setContent("<mpadded depth='-2'><msub><mi>g</mi><mi>i</mi></msub><mfence"\
                           "d open='(' close=')' separators=','> <mi> x </mi> <mi> y </m"\
                           "i> </mfenced> <mo>=</mo></mpadded>");
        label0->setFixedSize(label0->sizeHint());
        value0->setText("");
        label1->hide();
        value1->setText("");
        value1->hide();
    }
    else if(index==1)
    {
        condition->setContent("<mpadded depth='-2'><mo>&nabla;</mo><msub><mi>u</mi><mrow"\
                              "><mo>&#124;</mo><msub><mi>&Gamma;</mi><mi>i</mi></msub></"\
                              "mrow></msub><mo>&CenterDot;</mo><mi mathvariant='bold'>n<"\
                              "/mi><mo>=</mo><msub><mi>h</mi><mi>i</mi></msub></mpadded>"\
                              );
        condition->setFixedSize(condition->sizeHint());
        label0->setContent("<mpadded depth='-2'><msub><mi>h</mi><mi>i</mi></msub><mfence"\
                           "d open='(' close=')' separators=','> <mi> x </mi> <mi> y </m"\
                           "i> </mfenced><mo>=</mo></mpadded>");
        label0->setFixedSize(label0->sizeHint());
        value0->setText("");
        label1->hide();
        value1->setText("");
        value1->hide();
    }
    else
    {
        condition->setContent("<mpadded depth='-2'><mo>&nabla;</mo><msub><mi>u</mi><mrow"\
                              "><mo>&#124;</mo><msub><mi>&Gamma;</mi><mi>i</mi></msub></"\
                              "mrow></msub><mo>&CenterDot;</mo><mi mathvariant='bold'>n<"\
                              "/mi><mo>+</mo><msub><mi>&gamma;</mi><mi>i</mi></msub><msu"\
                              "b><mi>u</mi><mrow><mo>&#124;</mo><msub><mi>&Gamma;</mi><m"\
                              "i>i</mi></msub></mrow></msub><mo>=</mo><msub><mi>r</mi><m"\
                              "i>i</mi></msub></mpadded>");
        condition->setFixedSize(condition->sizeHint());
        label0->setContent("<mpadded depth='-2'><msub><mi>&gamma;</mi><mi>i</mi></msub><"\
                           "mfenced open='(' close=')' separators=','> <mi> x </mi> <mi>"\
                           "y </mi> </mfenced><mo>=</mo></mpadded>");
        label0->setFixedSize(label0->sizeHint());
        value0->setText("");
        label1->setContent("<mpadded depth='-2'><msub><mi>r</mi><mi>i</mi></msub><mfence"\
                           "d open='(' close=')' separators=','> <mi> x </mi> <mi> y </m"\
                           "i> </mfenced><mo>=</mo></mpadded>");
        label1->setFixedSize(label0->sizeHint());
        label1->show();
        value1->setText("");
        value1->show();
    }
};

void NewBoundaryConditionsDialog::checkInput()
{
    int index = conditions_list->currentIndex();

    // If input is uncomplete disable the add/modify button and show no message
    if(border_value->text().isEmpty() || value0->text().isEmpty())
    {
        add_mod->setEnabled(false);
        message->setText("");
        return;
    }
    if(index>1 && value1->text().isEmpty())
    {
        add_mod->setEnabled(false);
        message->setText("");
        return;
    }

    // check if border value is integer
    bool is_int;
    int border = border_value->text().toInt(&is_int);
    if (!is_int || border<0)
    {
        add_mod->setEnabled(false);
        message->setText("Border number must be a non-negative integer");
        return;
    }
    if (borders.contains(border))
    {
        if((selected_row && border!=borders[selected_row-1]) || !selected_row)
        {
            add_mod->setEnabled(false);
            message->setText("Border number must be unique");
            return;
        }
    }
    //check value0
    QString f = "( function(x,y) { return "+value0->text()+"; } )";
    QScriptSyntaxCheckResult result(QScriptEngine::checkSyntax(f));
    if(result.state() == QScriptSyntaxCheckResult::Intermediate)
    {
        button_save->setEnabled(false);
        return;
    }
    if(result.state() == QScriptSyntaxCheckResult::Error)
    {
        message->setText("Error in function definition");
        button_save->setEnabled(false);
        return;
    }
    if(index>1)
    {
        QString f = "( function(x,y) { return "+value1->text()+"; } )";
        QScriptSyntaxCheckResult result(QScriptEngine::checkSyntax(f));
        if(result.state() == QScriptSyntaxCheckResult::Intermediate)
        {
            button_save->setEnabled(false);
            return;
        }
        if(result.state() == QScriptSyntaxCheckResult::Error)
        {
            message->setText("Error in function definition");
            button_save->setEnabled(false);
            return;
        }
    }
    // if all checks are succesfully passed enable the add button
    add_mod->setEnabled(true);
    message->setText("");
    return;
};

void NewBoundaryConditionsDialog::addModifyInput()
{
    int row;
    int border = border_value->text().toInt();
    if(selected_row)
    {
        row = selected_row-1;
        borders[selected_row-1] = border;
    }
    else
    {
        row = borders.size();
        borders.push_back(border);
    }

    QString value;
    if(!selected_row)
    {
        QFont monospace("Courier");
        monospace.setStyleHint(QFont::TypeWriter);
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem);
        table->item(row, 0)->setFont(monospace);
        table->item(row, 0)->setFlags(Qt::ItemIsSelectable);
    }
    switch (conditions_list->currentIndex())
    {
    case 0:
        value = QString::number(border) + "\t DIRICHLET \t" + value0->text().remove(QRegExp("\\s+"));
        break;
    case 1:
        value = QString::number(border) + "\t NEUMANN   \t" + value0->text().remove(QRegExp("\\s+"));
        break;
    case 2:
        value = QString::number(border) + "\t ROBIN     \t" + value0->text().remove(QRegExp("\\s+")) + "\t" + value1->text().remove(QRegExp("\\s+"));
        break;
    }
    table->item(row, 0)->setText(value);
    table->clearSelection();
    selected_row = 0;
    border_value->setText("");
    value0->setText("");
    value1->setText("");
    add_mod->setText("+");
    add_mod->setEnabled(false);
    del->setEnabled(false);
    border_value->setFocus();
};

void NewBoundaryConditionsDialog::deleteInput()
{
    table->clearSelection();
    table->hideRow(selected_row-1);
    border_value->setText("");
    value0->setText("");
    value1->setText("");
    add_mod->setText("+");
    add_mod->setEnabled(false);
    del->setEnabled(false);
    border_value->setFocus();
    selected_row = 0;
};

void NewBoundaryConditionsDialog::save()
{
    if(!temp_file.open())
    {
        QMessageBox message;
        message.setText("Error saving Boundary Conditions.");
        message.exec();
        return;
    }
    QTextStream out(&temp_file);
    for(int i=0; i<table->rowCount(); ++i)
        out << table->item(i,0)->text() + "\n";
    temp_file.close();
    done(true);
};

void NewBoundaryConditionsDialog::selectRow()
{
    int row = table->currentRow();
    selected_row = row+1;
    QStringList values(table->item(row,0)->text().split("\t",QString::SkipEmptyParts));
    values.replaceInStrings(QRegExp("\\s+"),"");
    // Set the input to the modify mode
    add_mod->setText(QChar(0x2217));
    add_mod->setEnabled(true);
    del->setEnabled(true);
    // Set input values to the corresponding ones in the table
    this->border_value->setText(values[0]);
    if(values[1]=="DIRICHLET")
    {
        conditions_list->setCurrentIndex(0);
        value0->setText(values[2]);
    }
    if(values[1]=="NEUMANN")
    {
        conditions_list->setCurrentIndex(0);
        value0->setText(values[2]);
    }
    if(values[1]=="ROBIN")
    {
        conditions_list->setCurrentIndex(0);
        value0->setText(values[2]);
        value1->setText(values[3]);
    }
};

void NewBoundaryConditionsDialog::checkName()
{
    QString stringName = line_name->text();
    if(stringName.isEmpty())
    {
        message->setText("");
        button_save->setEnabled(false);
    }
    else if(boundary_conditions.contains(stringName))
    {
        message->setText("Existing file will be overwritten");
        button_save->setEnabled(true);
    }
    else
    {
        message->setText("");
        button_save->setEnabled(true);
    }
};
