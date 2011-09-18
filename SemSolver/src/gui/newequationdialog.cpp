#include "newequationdialog.hpp"

#include <QMessageBox>
#include <QScriptEngine>
#include <QScriptSyntaxCheckResult>

NewEquationDialog::NewEquationDialog(QWidget *parent,
                                     const QStringList &existing_equations)
    : QDialog(parent), equations(existing_equations)
{
    layout = new QVBoxLayout;
    layout_problem = new QHBoxLayout;
    layout_inputs = new QGridLayout;
    layout_buttons = new QHBoxLayout;
    label_problem = new QLabel;
    problem_list = new QComboBox;
    problem = new Formula;
    label_diffusion = new Formula;
    diffusion = new QLineEdit;
    label_convection_x = new Formula;
    convection_x = new QLineEdit;
    label_convection_y = new Formula;
    convection_y = new QLineEdit;
    label_reaction = new Formula;
    reaction = new QLineEdit;
    label_source = new Formula;
    source = new QLineEdit;
    message = new QLabel;
    label_name = new QLabel;
    line_name = new QLineEdit;
    cancel = new QPushButton;
    button_save = new QPushButton;
    label_problem->setText("Problem:");
    problem_list->addItem("Diffusion-Convection-Reaction");
    problem->setContent("<mrow><mo>-</mo><mo>&Del;&CenterDot;</mo><mfenced open='(' clos"\
                        "e=')'><mrow><mi>&mu;</mi><mo>&Del;</mo> <mi>u</mi></mrow></mfen"\
                        "ced><mo>+</mo><mi mathvariant='bold'>b</mi><mo>&CenterDot;</mo>"\
                        "<mo>&Del;</mo> <mi>u</mi><mo>+</mo><mi>&sigma;</mi><mi>u</mi><m"\
                        "o>=</mo><mi>f</mi></mrow>");
    label_diffusion->setContent("<mrow><mi>&mu;</mi> <mfenced open='(' close=')' separat"\
                                "ors=','> <mi> x </mi> <mi> y </mi> </mfenced> <mo>=</mo"\
                                "></mrow>");
    label_convection_x->setContent("<mrow><msub><mi>b</mi><mi>x</mi></msub> <mfenced ope"\
                                   "n='(' close=')' separators=','> <mi> x </mi> <mi> y "\
                                   "</mi> </mfenced> <mo>=</mo></mrow>");
    label_convection_y->setContent("<mrow><msub><mi>b</mi><mi>y</mi></msub> <mfenced ope"\
                                   "n='(' close=')' separators=','> <mi> x </mi> <mi> y "\
                                   "</mi> </mfenced> <mo>=</mo></mrow>");
    label_reaction->setContent("<mrow><mi>&sigma;</mi> <mfenced open='(' close=')' separ"\
                               "ators=','> <mi> x </mi> <mi> y </mi> </mfenced> <mo>=</m"\
                               "o></mrow>");
    label_source->setContent("<mrow><mi>f</mi> <mfenced open='(' close=')' separators=',"\
                             "'> <mi> x </mi> <mi> y </mi> </mfenced> <mo>=</mo></mrow>");
    label_name->setText("Name:");
    cancel->setText("&Cancel");
    button_save->setText("&Save");
    button_save->setEnabled(false);
    layout_problem->addWidget(label_problem);
    layout_problem->addWidget(problem_list);
    layout_problem->addStretch();

    layout_inputs->addWidget(label_diffusion,0,0);
    layout_inputs->addWidget(diffusion,0,1);
    layout_inputs->addWidget(label_convection_x,1,0);
    layout_inputs->addWidget(convection_x,1,1);
    layout_inputs->addWidget(label_convection_y,2,0);
    layout_inputs->addWidget(convection_y,2,1);
    layout_inputs->addWidget(label_reaction,3,0);
    layout_inputs->addWidget(reaction,3,1);
    layout_inputs->addWidget(label_source,4,0);
    layout_inputs->addWidget(source,4,1);

    layout_buttons->addWidget(label_name);
    layout_buttons->addWidget(line_name);
    layout_buttons->addWidget(cancel);
    layout_buttons->addWidget(button_save);

    layout->addLayout(layout_problem);
    layout->addSpacing(24);
    layout->addWidget(problem);
    layout->addSpacing(24);
    layout->addLayout(layout_inputs);
    layout->addWidget(message);
    layout->addLayout(layout_buttons);

    setLayout(layout);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle("New Equation...");

    connect(diffusion, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(convection_x, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(convection_y, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(reaction, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(source, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_save, SIGNAL(clicked()), this, SLOT(save()));
};

NewEquationDialog::~NewEquationDialog()
{
    delete problem_list;
    delete problem;
    delete label_diffusion;
    delete diffusion;
    delete label_convection_x;
    delete convection_x;
    delete label_convection_y;
    delete convection_y;
    delete label_reaction;
    delete reaction;
    delete label_source;
    delete source;
    delete message;
    delete label_name;
    delete line_name;
    delete cancel;
    delete button_save;
    delete layout_problem;
    delete layout_inputs;
    delete layout_buttons;
    delete label_problem;
    delete layout;
};

void NewEquationDialog::save()
{
    if(!temp_file.open())
    {
        QMessageBox message;
        message.setText("Error saving Equation.");
        message.exec();
        return;
    }
    QTextStream out(&temp_file);
    out << "DIFFUSION_CONVECTION_REACTION \n";
    if(!diffusion->text().isEmpty())
        out << "DIFFUSION \t" << diffusion->text().remove(QRegExp("\\s+")) << "\n";
    if(convection_x->text().isEmpty())
    {
        if(!convection_y->text().isEmpty())
            out << "CONVECTION \t" << "0 \t" << convection_y->text().remove(QRegExp("\\s+")) << "\n";
    }
    else if(!convection_y->text().isEmpty())
        out << "CONVECTION \t" << convection_x->text().remove(QRegExp("\\s+")) << "\t" << convection_y->text() << "\n";
    else
        out << "CONVECTION \t" << convection_x->text().remove(QRegExp("\\s+")) << "\t 0 \n";
    if(!reaction->text().isEmpty())
        out << "REACTION \t" << reaction->text().remove(QRegExp("\\s+")) << "\n";
    if(!source->text().isEmpty())
        out << "FORCING \t" << source->text().remove(QRegExp("\\s+")) << "\n";
    temp_file.close();
    done(true);
};

void NewEquationDialog::checkInput()
{
    QString mi = "( function(x,y) { return "+diffusion->text()+"; } )";
    QString bx = "( function(x,y) { return "+convection_x->text()+"; } )";
    QString by = "( function(x,y) { return "+convection_y->text()+"; } )";
    QString sigma = "( function(x,y) { return "+reaction->text()+"; } )";
    QString f = "( function(x,y) { return " +source->text()+"; } )";
    QScriptSyntaxCheckResult result(QScriptEngine::checkSyntax(mi));
    if(result.state() == QScriptSyntaxCheckResult::Intermediate)
    {
        button_save->setEnabled(false);
        return;
    }
    if(result.state() == QScriptSyntaxCheckResult::Error)
    {
        message->setText("Error in diffusion definition");
        button_save->setEnabled(false);
        return;
    }
    result = QScriptEngine::checkSyntax(bx);
    if(result.state() == QScriptSyntaxCheckResult::Intermediate)
    {
        button_save->setEnabled(false);
        return;
    }
    if(result.state() == QScriptSyntaxCheckResult::Error)
    {
        message->setText("Error in the definition of the x component of the convection");
        button_save->setEnabled(false);
        return;
    }
    result = QScriptEngine::checkSyntax(by);
    if(result.state() == QScriptSyntaxCheckResult::Intermediate)
    {
        button_save->setEnabled(false);
        return;
    }
    if(result.state() == QScriptSyntaxCheckResult::Error)
    {
        message->setText("Error in the definition of the y component of the convection");
        button_save->setEnabled(false);
        return;
    }
    result = QScriptEngine::checkSyntax(sigma);
    if(result.state() == QScriptSyntaxCheckResult::Intermediate)
    {
        button_save->setEnabled(false);
        return;
    }
    if(result.state() == QScriptSyntaxCheckResult::Error)
    {
        message->setText("Error in reaction definition");
        button_save->setEnabled(false);
        return;
    }
    result = QScriptEngine::checkSyntax(f);
    if(result.state() == QScriptSyntaxCheckResult::Intermediate)
    {
        button_save->setEnabled(false);
        return;
    }
    if(result.state() == QScriptSyntaxCheckResult::Error)
    {
        message->setText("Error in forcing definition");
        button_save->setEnabled(false);
        return;
    }
    checkName();
};

void NewEquationDialog::checkName()
{
    QString stringName = line_name->text();
    if(stringName.isEmpty())
    {
        message->setText("");
        button_save->setEnabled(false);
    }
    else if(equations.contains(stringName))
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

