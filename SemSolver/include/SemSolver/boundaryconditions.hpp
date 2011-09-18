#ifndef BOUNDARYCONDITIONS_HPP
#define BOUNDARYCONDITIONS_HPP

#include <QString>
#include <QStringList>

namespace SemSolver
{
    template<int d, class X>
    class BoundaryConditions;
};

#include <map>
#include <vector>

#include <SemSolver/function.hpp>
#include <SemSolver/point.hpp>
#include <SemSolver/semgeometry.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling boundary conditions on a SemGeometry
    //! \param d Dimension of the space on which the geometry lives
    /*! \param X Must be a type for which operations +, -, * and / are defined with
        semantics (approximately) corresponding to those of a field in a mathematical
        sense. Note that, strictly speaking, the built-in type int does not fullfil the
        requirements on a field type, since ints correspond to elements of a ring rather
        than a field, especially operation / is not the inverse of *.                   */
    template<int d, class X>
    class BoundaryConditions
    {
    };

    //! \brief Class for handling boundary conditions on a 2D SemGeometry
    /*! \param X Must be a type for which operations +, -, * and / are defined with
        semantics (approximately) corresponding to those of a field in a mathematical
        sense. Note that, strictly speaking, the built-in type int does not fullfil the
        requirements on a field type, since ints correspond to elements of a ring rather
        than a field, especially operation / is not the inverse of *.                   */
    template<class X>
    class BoundaryConditions<2,X>
    {
    public:
        //! \brief Enumeration of the available boundary condition types
        enum Type
        {
            UNDEFINED,
            //! \brief Dirichlet condition
            //! \f$u=g\f$ on border \f$\Gamma_i\f$
            DIRICHLET,
            //! \brief Neumann condition
            //! \f$\nabla u \cdot \mathbf n = h\f$ on border \f$\Gamma_i\f$
            NEUMANN,
            //! \brief Robin condition
            //! \f$u+\gamma\nabla u \cdot\mathbf n = r\f$ on border \f$\Gamma_i\f$
            ROBIN
        };

        typedef Function< Point<2,X>, X > const * FunctionPtr;
        typedef std::map<int, FunctionPtr> FunctionsMap;
        typedef std::map<int, Type > TypesMap;
        typedef typename FunctionsMap::const_iterator FunctionConstIterator;

    private:
        // types map
        TypesMap _types;

        // ceofficients
        FunctionsMap _g;
        FunctionsMap _h;
        FunctionsMap _gamma;
        FunctionsMap _r;

    public:

        inline BoundaryConditions();

        BoundaryConditions(TypesMap const &types,
                           FunctionsMap const *g,
                           FunctionsMap const *h,
                           FunctionsMap const *gamma,
                           FunctionsMap const *r);

        template<class Y>
        BoundaryConditions(BoundaryConditions<2,Y> const &conditions);

        inline int conditions() const;

        inline Type const &borderType(int const &index) const;

        inline FunctionPtr dirichletData(int const &index) const;

        inline FunctionPtr neumannData(int const &index) const;

        inline FunctionPtr robinCoefficient(int const &index) const;

        inline FunctionPtr robinData(int const &index) const;

        void setBorder(int const &index,
                       Type const &type,
                       FunctionPtr const f1=0,
                       FunctionPtr const f2=0);

        QStringList labels() const;

        QStringList mmls() const;

        void clear();
    };
};

//! \brief Construct empty boundary conditions for a specific geometry
template<class X>
SemSolver::BoundaryConditions<2, X>::BoundaryConditions()
{
};

//! \brief Construct specific boundary conditions for a specific geometry
//! \param types Vector of the condition types for each geometry border
//! \param g Border datas for Dirichlet conditions
//! \param h Border datas for Neumann conditions
//! \param gamma Border coefficient for Robin conditions
//! \param r Border datas for Robin conditions
template<class X>
SemSolver::BoundaryConditions<2, X>::BoundaryConditions(TypesMap const &types,
                                                        FunctionsMap const *g,
                                                        FunctionsMap const *h,
                                                        FunctionsMap const *gamma,
                                                        FunctionsMap const *r)
{
    _types(types);
    for(typename TypesMap::const_iterator it=types.begin(); it!=types.end(); ++it)
    {
        switch(it->second)
        {
        case DIRICHLET:
#ifdef SEMDEBUG
            if(!g)
                qFatal("SemSolver::BoundaryConditions::BoundaryConditions - ERROR - tryi"\
                       "ng to access null pointer g.");
#endif
            _g[it->first] = g[it->first];
            break;
        case NEUMANN:
#ifdef SEMDEBUG
            if(!h)
                qFatal("SemSolver::BoundaryConditions::BoundaryConditions - ERROR - tryi"\
                       "ng to access null pointer h.");
#endif
            _h[it->first] = h[it->first];
            break;
        case ROBIN:
#ifdef SEMDEBUG
            if(!gamma)
                qFatal("SemSolver::BoundaryConditions::BoundaryConditions - ERROR - tryi"\
                       "ng to access null pointer gamma.");
            if(!r)
                qFatal("SemSolver::BoundaryConditions::BoundaryConditions - ERROR - tryi"\
                       "ng to access null pointer r.");
#endif
            _gamma[it->first] = gamma[it->first];
            _r[it->first] = r[it->first];
            break;
        default:
            _types[it->first] = UNDEFINED;
        }
    }
};

//! \brief Copy constructor
//! \param conditions Boundary conditions to be copied
template<class X>
template<class Y>
SemSolver::BoundaryConditions<2, X>::BoundaryConditions(
        BoundaryConditions<2, Y> const &conditions)
{
    _types = conditions._types;
    _g = conditions._g;
    _h = conditions._h;
    _gamma = conditions._gamma;
    _r = conditions._r;
};

//! \brief Get the number of boundary conditions
//! \return The conditions number
template<class X>
inline int SemSolver::BoundaryConditions<2, X>::conditions() const
{
    return _types.size();
};

//! \brief Get the condition type on a border
//! \param index Of the border
//! \return The border Type
template<class X>
inline typename SemSolver::BoundaryConditions<2, X>::Type const &
        SemSolver::BoundaryConditions<2, X>::borderType(int const &index) const
{
    typename TypesMap::const_iterator it = _types.find(index);
#ifdef SEMDEBUG
    if(it==_types.end() )
        qFatal("SemSolver::BoundaryConditions::borderType - ERROR : no border at index.");
#endif
    return it->second;
};

//! \brief Get the Dirichlet data \f$g\f$ on a border
//! \return Pointer to a scalar function
//! \param index of the border
template<class X>
inline SemSolver::Function< SemSolver::Point<2, X>, X > const *
        SemSolver::BoundaryConditions<2, X>::dirichletData(int const &index) const
{
    FunctionConstIterator it = _g.find(index);
#ifdef SEMDEBUG
    if(it==_g.end() )
        qFatal("SemSolver::BoundaryConditions::dirichletData - ERROR : the border at ind"\
               "ex is not Dirichlet.");
#endif
    return it->second;
};

//! \brief Get the Neumann data \f$h\f$ on a border
//! \param index of the border
//! \return Pointer to a scalar function
template<class X>
inline SemSolver::Function< SemSolver::Point<2, X>, X > const *
        SemSolver::BoundaryConditions<2, X>::neumannData(int const &index) const
{
    FunctionConstIterator it = _h.find(index);
#ifdef SEMDEBUG
    if(it==_h.end() )
        qFatal("SemSolver::BoundaryConditions::neumannData - ERROR : the border at index"\
               " is not Neumann.");
#endif
    return it->second;
};

//! \brief Get the Robin coefficient \f$\gamma\f$ on a border
//! \param index of the border
//! \return Pointer to a scalar function
template<class X>
inline SemSolver::Function< SemSolver::Point<2, X>, X > const *
        SemSolver::BoundaryConditions<2, X>::robinCoefficient(int const &index) const
{
    FunctionConstIterator it = _gamma.find(index);
#ifdef SEMDEBUG
    if(it==_gamma.end() )
        qFatal("SemSolver::BoundaryConditions::robinCoefficient - ERROR : the border at "\
               "index is not Robin.");
#endif
    return it->second;
};

//! \brief Get the Robin data \f$r\f$ on a border
//! \param index of the border
//! \return Pointer to a scalar function
template<class X>
inline SemSolver::Function< SemSolver::Point<2, X>, X > const *
        SemSolver::BoundaryConditions<2, X>::robinData(int const &index) const
{
    FunctionConstIterator it = _r.find(index);
#ifdef SEMDEBUG
    if(it==_r.end() )
        qFatal("SemSolver::BoundaryConditions::robinData - ERROR : the border at index i"\
               "s not Robin.");
#endif
    return it->second;
};

//! \brief Set condition on a border
//! \param index Index of the border
//! \param type Type of the border
/*! \param f1 Pointer to scalar function, it is used as border data for Dirichlet and
    Neumann conditions and as coefficient for Robin one, it is ignored otherwise        */
/*! \param f2 Pointer to scalar function, it is used as border data for Robin conditions,
     it is ignored otherwise                                                            */
template<class X>
void SemSolver::BoundaryConditions<2, X>::setBorder(int const &index,
               Type const &type,
               FunctionPtr const f1,
               FunctionPtr const f2)
{
    _types[index] = type;
    switch(type)
    {
    case DIRICHLET:
#ifdef SEMDEBUG
        if(!f1)
            qFatal("SemSolver::BoundaryConditions::setBorder - ERROR : trying to access "\
                   "null pointer f1.");
#endif
        _g[index] = f1;
        break;
    case NEUMANN:
#ifdef SEMDEBUG
        if(!f1)
            qFatal("SemSolver::BoundaryConditions::setBorder - ERROR : trying to access "\
                   "null pointer f1.");
#endif
        _h[index] = f1;
        break;
    case ROBIN:
#ifdef SEMDEBUG
        if(!f1)
            qFatal("SemSolver::BoundaryConditions::setBorder - ERROR : trying to access "\
                   "null pointer f1.");
        if(!f2)
            qFatal("SemSolver::BoundaryConditions::setBorder - ERROR : trying to access "\
                   "null pointer f2.");
#endif
        _gamma[index] = f1;
        _r[index] = f2;
        break;
    default:
        _types[index] = UNDEFINED;
    }
};

//! \brief Get the list of boundary labels
/*! \return QStringList containing one QString for each edge given by segment id number
    preceded by an 'S'                                                                  */
template<class X>
QStringList SemSolver::BoundaryConditions<2, X>::labels() const
{
    QStringList list;
    for(typename TypesMap::const_iterator it=_types.begin(); it!=_types.end(); ++it)
        list << "S" + QString::number(it->first);
    return list;
};

//! \brief Get the list of boundary conditions in Mathematical Markup Language format
/*! \return QStringList containing one QString for each boundary condition in MathML
    notation                                                                            */
template<class X>
QStringList SemSolver::BoundaryConditions<2, X>::mmls() const
{
    QStringList list;
    for(typename TypesMap::const_iterator it=_types.begin(); it!=_types.end(); ++it)
    {
        QString mml;
        switch(it->second)
        {
        case SemSolver::BoundaryConditions<2, X>::DIRICHLET:
            mml = "<mpadded depth='-2'><msub><mi>u</mi><mrow><mo>&#124;</mo><msub><mi>&G"\
                  "amma;</mi><mi>i</mi></msub></mrow></msub><mo>=</mo><msub><mi>g</mi><m"\
                  "i>i</mi></msub><mtext>,&nbsp;&nbsp;</mtext><msub><mi>g</mi><mi>i</mi>"\
                  "</msub><mfenced open='(' close=')' separators=','> <mi> x </mi> <mi> "\
                  "y </mi> </mfenced> <mo>=</mo>"
                  + dirichletData(it->first)->mml() +
                  "</mpadded>";
            break;
        case SemSolver::BoundaryConditions<2, X>::NEUMANN:
            mml = "<mpadded depth='-2'><mo>&nabla;</mo><msub><mi>u</mi><mrow><mo>&#124;<"\
                  "/mo><msub><mi>&Gamma;</mi><mi>i</mi></msub></mrow></msub><mo>&CenterD"\
                  "ot;</mo><mi mathvariant='bold'>n</mi><mo>=</mo><msub><mi>h</mi><mi>i<"\
                  "/mi></msub><mtext>,&nbsp;&nbsp;</mtext><msub><mi>h</mi><mi>i</mi></ms"\
                  "ub><mfenced open='(' close=')' separators=','> <mi> x </mi> <mi> y </"\
                  "mi> </mfenced><mo>=</mo>"
                  + neumannData(it->first)->mml() +
                  "</mpadded>";
            break;
        case SemSolver::BoundaryConditions<2, X>::ROBIN:
            mml = "<mpadded depth='-2'><mo>&nabla;</mo><msub><mi>u</mi><mrow><mo>&#124;<"\
                  "/mo><msub><mi>&Gamma;</mi><mi>i</mi></msub></mrow></msub><mo>&CenterD"\
                  "ot;</mo><mi mathvariant='bold'>n</mi><mo>+</mo><msub><mi>&gamma;</mi>"\
                  "<mi>i</mi></msub><msub><mi>u</mi><mrow><mo>&#124;</mo><msub><mi>&Gamm"\
                  "a;</mi><mi>i</mi></msub></mrow></msub><mo>=</mo><msub><mi>r</mi><mi>i"\
                  "</mi></msub><mtext>,&nbsp;&nbsp;</mtext><msub><mi>&gamma;</mi><mi>i</"\
                  "mi></msub><mfenced open='(' close=')' separators=','> <mi> x </mi> <m"\
                  "i> y </mi> </mfenced><mo>=</mo>"
                  + robinCoefficient(it->first)->mml() +
                  "<mtext>,&nbsp;&nbsp;</mtext><msub><mi>r</mi><mi>i</mi></msub><mfenced"\
                  " open='(' close=')' separators=','> <mi> x </mi> <mi> y </mi> </mfenc"\
                  "ed><mo>=</mo>"
                  + robinData(it->first)->mml() +
                  "</mpadded>";
            break;
        default:
            mml = "<mtext>none</mtext>";
        }
        list << mml;
    }
    return list;
};

//! \brief Free boundary conditions' content
template<class X>
void SemSolver::BoundaryConditions<2, X>::clear()
{
    _types.clear();
    _g.clear();
    _h.clear();
    _gamma.clear();
    _r.clear();
};

#endif // BOUNDARYCONDITIONS_HPP

