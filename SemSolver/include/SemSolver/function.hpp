#ifndef FUNCTION_HPP
#define FUNCTION_HPP

namespace SemSolver
{
    template<class X, class Y>
    class Function;
};


//! \brief Project main namespace
namespace SemSolver
{

    //! \brief Prototype class for mathematical functions \f$:X\to Y\f$
    //! \param X The type of the domain
    //! \param Y The type of the codomain
    template<class X, class Y>
    class Function
    {

    public:

        //! \brief Default constructor
        Function() {};

        //! \brief Destructor
        virtual ~Function() {};

        //! \brief Evaluate function at a point
        //! \return value assumed by function at x
        virtual Y evaluate(X const &) const { return Y(); };

        //! \brief Get function definition in Mathematical Markup Language notation
        //! \return QString of function definition in MathML format
        virtual QString mml() const { return ""; };
    };
};

#endif // FUNCTION_HPP
