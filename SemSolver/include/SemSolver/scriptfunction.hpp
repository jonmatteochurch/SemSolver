#ifndef SCRIPTFUNCTION_HPP
#define SCRIPTFUNCTION_HPP

namespace SemSolver
{
    template<class X, class Y>
    class ScriptFunction;
};

#include <cmath>

#include <SemSolver/point.hpp>
#include <SemSolver/function.hpp>
#include <SemSolver/vector.hpp>

#include <QString>
#include <QStringList>
#include <QScriptEngine>

namespace SemSolver
{
    /*! \brief Class for handling function from Euclidean space X^d to scalar space Y
        defined in scripts */
    /*! Such functions are evaluated at run time and allow users to speecify the function
        definition during execution for example by using external ECMA Script files. */
    template<int d, class X, class Y>
    class ScriptFunction< Point<d, X>, Y >
        : public Function< Point<d, X>, Y >
    {
        QString script;
        QString program;
        QScriptEngine *engine;
    public:
        //! \brief Constructor
        /*! \param string Function definition in ECMA Script language. Variables are
            assumed to be x0, x1, ..., xd-1. To the function  associated the ECMA Script
            "function f(x0, x1, ..., xd-1) { return string }". For ease of use,
            mathematical function of Math object are redifined, in this way sin(x) can be
            used instead of Math.sin(x) */
        ScriptFunction(QString const &string);

        //! \brief Destructor
        ~ScriptFunction();

        //! \brief Compute function value at a point
        Y evaluate(const Point<d, X> &point) const;

        //! \brief Get function definition in Mathematical Markup Language notation
        //! \return QString of function definition in MathML format
        QString mml() const;
    };

    /*! \brief Class for handling function from Euclidean space X^d to Vectorial space Y^n
        defined inscripts */
    /*! Such functions are evaluated at run time and allow users to speecify the function
        definition during execution for example by using external ECMA Script files. */
    template<int d, class X, class Y>
    class ScriptFunction< Point<d, X>, Vector<Y> >
        : public Function< Point<d, X>, Vector<Y> >
    {
        QStringList scripts;
        QString program;
        QScriptEngine *engine;
    public:
        //! \brief Constructor
        /*! \param strings Function component definitions in ECMA Script language.
            Variables are assumed to be x0, x1, ..., xd-1. To the function is associated
            the ECMA Script "function f0(x0, x1, ..., xd-1) { return string[0] }, function
            f1(x0, x1, ..., xd-1) { return string[1] }, ..., function
            fn(x0, x1, ..., xd-1) { return string[n] }". For ease of use, mathematical
            function of Math object are redifined, in this way sin(x) can be used instead
            of Math.sin(x) */
        ScriptFunction(QStringList const &strings);

        //! \brief Destructor
        ~ScriptFunction();

        //! \brief Compute function value at a point
        Vector<Y> evaluate(const Point<d, X> &point) const;

        //! \brief Get function definition in Mathematical Markup Language notation
        //! \return QString of function definition in MathML format
        QString mml() const;
    };

    /*! \brief Class for handling function from 2D Euclidean space X^2 to scalar space Y
    defined in scripts */
    /*! Such functions are evaluated at run time and allow users to speecify the function
        definition during execution for example by using external ECMA Script files. */
    template<class X, class Y>
    class ScriptFunction< Point<2, X>, Y >
        : public Function< Point<2, X>, Y >
    {
        QString script;
        QString program;
        QScriptEngine *engine;
    public:
        //! Constructor
        /*! \param string Function definition in ECMA Script language. Variables are
            assumed to be x, y. To the function  associated the ECMA Script
            "function f(x,y) { return string }". For ease of use, mathematical function of
            Math object are redifined, in this way sin(x) can be used instead of
            Math.sin(x) */
        ScriptFunction(QString const &string);

        //! \brief Destructor
        ~ScriptFunction();

        //! \brief Compute function value at a point
        Y evaluate(const Point<2, X> &point) const;

        //! \brief Get function definition in Mathematical Markup Language notation
        //! \return QString of function definition in MathML format
        QString mml() const;
    };

    /*! \brief Class for handling function from 2D Euclidean space X^2 to Vectorial space
        Y^n defined in scripts */
    /*! Such functions are evaluated at run time and allow users to speecify the function
        definition during execution for example by using external ECMA Script files. */
    template<class X, class Y>
    class ScriptFunction< Point<2, X>, Vector<Y> >
        : public Function< Point<2, X>, Vector<Y> >
    {
        QStringList scripts;
        QString program;
        QScriptEngine *engine;
    public:
        //! Constructor
        /*! \param strings Function component definitions in ECMA Script language.
            Variables are assumed to be x and y. To the function is associated the ECMA
            Script "function f0(x, y) { return string[0] }, function f1(x, y) { return
            string[1] }, ..., function fn(x, y) { return string[n] }". For ease of use,
            mathematical function of Math object are redifined, in this way
            sin(x) can be used instead of Math.sin(x) */
        ScriptFunction(QStringList const &strings);

        //! \brief Destructor
        ~ScriptFunction();

        //! \brief Compute function value at a point
        Vector<Y> evaluate(const Point<2, X> &point) const;

        //! \brief Get function definition in Mathematical Markup Language notation
        //! \return QString of function definition in MathML format
        QString mml() const;
    };
};

template<int d, class X, class Y>
SemSolver::ScriptFunction< SemSolver::Point<d, X>, Y >::ScriptFunction(
        const QString &string)
{
    script = string;
    engine = new QScriptEngine;
    program = "function f(";
    for(int i=0; i<d-1; ++i)
        program += "x" + QString::number(i) + ", ";
    program += "x" + QString::number(d) + ") { return " + script + "; }";
    engine->evaluate("function abs   (x)   { return Math.abs(x);     }"\
                     "function acos  (x)   { return Math.acos(x);    }"\
                     "function asin  (x)   { return Math.asin(x);    }"\
                     "function atan  (x)   { return Math.atan(x);    }"\
                     "function atan2 (x,y) { return Math.atan2(x,y); }"\
                     "function ceil  (x)   { return Math.ceil(x);    }"\
                     "function cos   (x)   { return Math.cos(x);     }"\
                     "function exp   (x)   { return Math.exp(x);     }"\
                     "function floor (x)   { return Math.floor(x);   }"\
                     "function log   (x)   { return Math.log(x);     }"\
                     "function max   (x,y) { return Math.max(x,y);   }"\
                     "function min   (x,y) { return Math.min(x,y);   }"\
                     "function pow   (x,y) { return Math.pow(x,y);   }"\
                     "function random()    { return Math.random();   }"\
                     "function round (x)   { return Math.round(x);   }"\
                     "function sin   (x)   { return Math.sin(x);     }"\
                     "function sqrt  (x)   { return Math.sqrt(x);    }"\
                     "function tan   (x)   { return Math.tan(x);     }"\
                     "var pi = Math.PI;                               "\
                     "var e  = Math.E ;                               ");
    engine->evaluate(program);
};

template<int d, class X, class Y>
SemSolver::ScriptFunction< SemSolver::Point<d, X>, Y >::~ScriptFunction()
{
    delete engine;
};

template<int d, class X, class Y>
Y SemSolver::ScriptFunction< SemSolver::Point<d, X>, Y >::evaluate(
        const SemSolver::Point<d, X> &point) const
{
    QString call = "f(";
    for(int i=0; i<d-1; ++i)
        call += QString::number(point.cartesian(i)) + ",";
    call += QString::number(point.cartesian(d-1)) + ")";
    QScriptValue value = engine->evaluate(call);
    return value.toNumber();
};

template<int d, class X, class Y>
QString SemSolver::ScriptFunction< SemSolver::Point<d, X>, Y >::mml() const
{
    return "<mtext> " + script + "</mtext>";
};

template<int d, class X, class Y>
SemSolver::ScriptFunction< SemSolver::Point<d, X>, SemSolver::Vector<Y> >::ScriptFunction(
        const QStringList &strings)
{
    scripts = strings;
    engine = new QScriptEngine;
    for(int j=0; j<scripts.size(); ++j)
    {
        program += "function f" + QString::number(j) +"(";
        for(int i=0; i<d-1; ++i)
            program += "x" + QString::number(i) + ", ";
        program += "x" + QString::number(d) + ") { return " + scripts[j] + "; }\n";
    }

    engine->evaluate("function abs   (x)   { return Math.abs(x);     }"\
                     "function acos  (x)   { return Math.acos(x);    }"\
                     "function asin  (x)   { return Math.asin(x);    }"\
                     "function atan  (x)   { return Math.atan(x);    }"\
                     "function atan2 (x,y) { return Math.atan2(x,y); }"\
                     "function ceil  (x)   { return Math.ceil(x);    }"\
                     "function cos   (x)   { return Math.cos(x);     }"\
                     "function exp   (x)   { return Math.exp(x);     }"\
                     "function floor (x)   { return Math.floor(x);   }"\
                     "function log   (x)   { return Math.log(x);     }"\
                     "function max   (x,y) { return Math.max(x,y);   }"\
                     "function min   (x,y) { return Math.min(x,y);   }"\
                     "function pow   (x,y) { return Math.pow(x,y);   }"\
                     "function random()    { return Math.random();   }"\
                     "function round (x)   { return Math.round(x);   }"\
                     "function sin   (x)   { return Math.sin(x);     }"\
                     "function sqrt  (x)   { return Math.sqrt(x);    }"\
                     "function tan   (x)   { return Math.tan(x);     }"\
                     "var pi = Math.PI;                               "\
                     "var e  = Math.E ;                               ");
    engine->evaluate(program);
};

template<int d, class X, class Y>
SemSolver::ScriptFunction<SemSolver::Point<d,X>, SemSolver::Vector<Y> >::~ScriptFunction()
{
    delete engine;
};

template<int d, class X, class Y>
SemSolver::Vector<Y> SemSolver::ScriptFunction<
        SemSolver::Point<d, X>, SemSolver::Vector<Y> >::evaluate(
                const SemSolver::Point<d, X> &point) const
{
    int l = scripts.size();
    Vector<Y> result(l);
    for(int j=0; j<l; ++j)
    {
        QString call = "f" + QString::number(j) + "(";
        for(int i=0; i<d-1; ++i)
            call += QString::number(point.cartesian(i)) + ",";
        call += QString::number(point.cartesian(d-1)) + ")";
        QScriptValue value = engine->evaluate(call);
        result[j] = value.toNumber();
    };
    return result;
};

template<int d, class X, class Y>
QString SemSolver::ScriptFunction<SemSolver::Point<d, X>, SemSolver::Vector<Y> >::mml(
        ) const
{
    QString mml = "<mfenced open='(' close=')'><mtable>";
    for(int i=0; i<scripts.size(); ++i)
        mml += "<mtr><mtd><mtext>" + scripts[i] + "</mtext></mtd></mtr>";
    mml += "</mtable></mfenced>";
    return mml;
};

template<class X, class Y>
SemSolver::ScriptFunction< SemSolver::Point<2, X>, Y >::ScriptFunction(
        const QString &string)
{
    script = string;
    engine = new QScriptEngine;
    program = "function f(x,y) { return " + script + "; }";
    engine->evaluate("function abs   (x)   { return Math.abs(x);     }"\
                     "function acos  (x)   { return Math.acos(x);    }"\
                     "function asin  (x)   { return Math.asin(x);    }"\
                     "function atan  (x)   { return Math.atan(x);    }"\
                     "function atan2 (x,y) { return Math.atan2(x,y); }"\
                     "function ceil  (x)   { return Math.ceil(x);    }"\
                     "function cos   (x)   { return Math.cos(x);     }"\
                     "function exp   (x)   { return Math.exp(x);     }"\
                     "function floor (x)   { return Math.floor(x);   }"\
                     "function log   (x)   { return Math.log(x);     }"\
                     "function max   (x,y) { return Math.max(x,y);   }"\
                     "function min   (x,y) { return Math.min(x,y);   }"\
                     "function pow   (x,y) { return Math.pow(x,y);   }"\
                     "function random()    { return Math.random();   }"\
                     "function round (x)   { return Math.round(x);   }"\
                     "function sin   (x)   { return Math.sin(x);     }"\
                     "function sqrt  (x)   { return Math.sqrt(x);    }"\
                     "function tan   (x)   { return Math.tan(x);     }"\
                     "var pi = Math.PI;                               "\
                     "var e  = Math.E ;                               ");
    engine->evaluate(program);
};

template<class X, class Y>
SemSolver::ScriptFunction< SemSolver::Point<2, X>, Y >::~ScriptFunction()
{
    delete engine;
};

template<class X, class Y>
Y SemSolver::ScriptFunction< SemSolver::Point<2, X>, Y >::evaluate(
        const SemSolver::Point<2, X> &point) const
{
    QString call = "f(" + QString::number(point.x()) + "," + QString::number(point.y()) \
                   + ")";
    QScriptValue value = engine->evaluate(call);
    return value.toNumber();
};

template<class X, class Y>
QString SemSolver::ScriptFunction< SemSolver::Point<2, X>, Y >::mml() const
{
    return "<mtext> " + script + "</mtext>";
};

template<class X, class Y>
SemSolver::ScriptFunction< SemSolver::Point<2, X>, SemSolver::Vector<Y> >::ScriptFunction(
        const QStringList &strings)
{
    scripts = strings;
    engine = new QScriptEngine;
    for(int j=0; j<scripts.size(); ++j)
        program += "function f" + QString::number(j) +"(x,y) { return " + scripts[j] + \
                   "; }\n";

    engine->evaluate("function abs   (x)   { return Math.abs(x);     }"\
                     "function acos  (x)   { return Math.acos(x);    }"\
                     "function asin  (x)   { return Math.asin(x);    }"\
                     "function atan  (x)   { return Math.atan(x);    }"\
                     "function atan2 (x,y) { return Math.atan2(x,y); }"\
                     "function ceil  (x)   { return Math.ceil(x);    }"\
                     "function cos   (x)   { return Math.cos(x);     }"\
                     "function exp   (x)   { return Math.exp(x);     }"\
                     "function floor (x)   { return Math.floor(x);   }"\
                     "function log   (x)   { return Math.log(x);     }"\
                     "function max   (x,y) { return Math.max(x,y);   }"\
                     "function min   (x,y) { return Math.min(x,y);   }"\
                     "function pow   (x,y) { return Math.pow(x,y);   }"\
                     "function random()    { return Math.random();   }"\
                     "function round (x)   { return Math.round(x);   }"\
                     "function sin   (x)   { return Math.sin(x);     }"\
                     "function sqrt  (x)   { return Math.sqrt(x);    }"\
                     "function tan   (x)   { return Math.tan(x);     }"\
                     "var pi = Math.PI;                               "\
                     "var e  = Math.E ;                               ");
    engine->evaluate(program);
};

template<class X, class Y>
SemSolver::ScriptFunction<SemSolver::Point<2,X>, SemSolver::Vector<Y> >::~ScriptFunction()
{
    delete engine;
};

template<class X, class Y>
SemSolver::Vector<Y> SemSolver::ScriptFunction<
        SemSolver::Point<2, X>, SemSolver::Vector<Y> >::evaluate(
                const SemSolver::Point<2, X> &point) const
{
    int l = scripts.size();
    Vector<Y> result(l);
    for(int j=0; j<l; ++j)
    {
        QString call = "f" + QString::number(j) + "(" + QString::number(point.x()) + ","
                       + QString::number(point.y()) + ")";
        QScriptValue value = engine->evaluate(call);
        result[j] = value.toNumber();
    };
    return result;
};

template<class X, class Y>
QString SemSolver::ScriptFunction<SemSolver::Point<2, X>, SemSolver::Vector<Y> >::mml(
        ) const
{
    QString mml = "<mfenced open='(' close=')'><mtable>";
    for(int i=0; i<scripts.size(); ++i)
        mml += "<mtr><mtd><mtext>" + scripts[i] + "</mtext></mtd></mtr>";
    mml += "</mtable></mfenced>";
    return mml;
};

#endif // SCRIPTFUNCTION_HPP
