#ifndef EQUATION_HPP
#define EQUATION_HPP

#include <QString>

namespace SemSolver
{
    template<int d, class X>
    class Equation;
}


//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Virtual class for handling general equation
    //! \param d Dimension of the space on which the equation is defined
    /*! \param X Must be a type for which operations +, -, * and / are defined with
        semantics (approximately) corresponding to those of a field in a mathematical
        sense. Note that, strictly speaking, the built-in type int does not fullfil the
        requirements on a field type, since ints correspond to elements of a ring rather
        than a field, especially operation / is not the inverse of *.                   */
    template<int d, class X>
    class Equation
    {
    public:
        //! \brief Enumeration of the avilable equation types
        enum Type
        {
            NONE,
            //! \brief Diffusion-Convection-Reaction steady equation
            DIFFUSION_CONVECTION_REACTION
        };

        //! \brief Default constructor
        Equation() {};

        //! \brief Destructor
        virtual ~Equation() {};

        //! \brief Get equation type
        virtual Type type() const { return NONE; };

        //! \brief Get equation in Mathematical Markup Language notation
        //! \return QString of equation in MathML format
        virtual QString mml() const { return ""; };
    };
};

#endif // EQUATION_HPP
