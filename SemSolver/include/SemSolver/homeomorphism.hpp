#ifndef HOMEOMORPHISM_HPP
#define HOMEOMORPHISM_HPP

#include <SemSolver/function.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Prototype class for mathematical homemorphism \f$:X\to Y\f$
    //! \param X The type of the domain
    //! \param Y The type of the codomain
    template<class X, class Y>
    class Homeomorphism
    : public Function<X,Y>
    {
    public:
        //! \brief Default constructor
        Homeomorphism() {};

        //! \brief Destructor
        virtual ~Homeomorphism() {};

        //! \brief Evaluate function inverse of a value y
        //! \return The preimage of y
        virtual X evaluateInverse(Y const &)
        {
            return X();
        };
    };
};

#endif // HOMEOMORPHISM_HPP
