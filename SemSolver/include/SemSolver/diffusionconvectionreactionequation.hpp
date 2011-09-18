#ifndef DIFFUSIONCONVECTIONREACTIONEQUATION_HPP
#define DIFFUSIONCONVECTIONREACTIONEQUATION_HPP

#include <QDebug>

namespace SemSolver
{
    template<int d, class X>
    class DiffusionConvectionReactionEquation;
};

#include <SemSolver/equation.hpp>
#include <SemSolver/function.hpp>
#include <SemSolver/point.hpp>
#include <SemSolver/vector.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling Diffusion-Convection-Reaction steady equation
    //! \param d Dimension of the space on which the equation is defined
    /*! \param X Must be a type for which operations +, -, * and / are defined with
        semantics (approximately) corresponding to those of a field in a mathematical
        sense. Note that, strictly speaking, the built-in type int does not fullfil the
        requirements on a field type, since ints correspond to elements of a ring rather
        than a field, especially operation / is not the inverse of *.                   */
    template<int d, class X>
    class DiffusionConvectionReactionEquation
        : public Equation<d, X>
    {
        // coefficients
        Function< Point<d, X>, X >         *_diffusion;
        Function< Point<d, X>, Vector<X> > *_convection;
        Function< Point<d, X>, X >         *_reaction;
        Function< Point<d, X>, X >         *_forcing;

    public:

        typedef typename Equation<d, X>::Type Type;

        //! \brief Default constructor
        //! Construct an emty equation
        DiffusionConvectionReactionEquation()
        {
            _diffusion = 0;
            _convection = 0;
            _reaction = 0;
            _forcing = 0;
        };

        //! \brief Destructor
        ~DiffusionConvectionReactionEquation()
        {
            delete _diffusion;
            delete _convection;
            delete _reaction;
            delete _forcing;
        };

        //! \brief Get the equation type
        //! \return DIFFUSION_CONVECTION_REACTION Equation::Type
        inline Type type() const
        {
            return Equation<d, X>::DIFFUSION_CONVECTION_REACTION;
        };

        QString mml() const;

        //! \brief Set the diffusion coefficient
        //! \param diffusion Pointer to function from X^d to X
        inline void setDiffusion(Function< Point<d, X>, X > *diffusion)
        {
            delete _diffusion;
            _diffusion = diffusion;
        };

        //! \brief Set the convection coefficient
        //! \param convection Pointer to function from X^d to X
        inline void setConvection(Function< Point<d, X>, Vector<X> > *convection)
        {
            delete _convection;
            _convection = convection;
        };

        //! \brief Set the reaction coefficient
        //! \param reaction Pointer to function from X^d to X
        inline void setReaction(Function< Point<d, X>, X > *reaction)
        {
            delete _reaction;
            _reaction = reaction;
        };

        //! \brief Set the forcing term
        //! \param forcing Pointer to function from X^d to X
        inline void setForcing(Function< Point<d, X>, X > *forcing)
        {
            delete _forcing;
            _forcing = forcing;
        };

        //! \brief Get the diffusion coefficient
        //! \return Pointer to function from X^d to X
        inline Function< Point<d, X>, X > *diffusion() const
        {
            return _diffusion;
        };

        //! \brief Get the convection coefficient
        //! \return Pointer to function from X^d to X
        inline Function< Point<d, X>, Vector<X> > *convection() const
        {
            return _convection;
        };

        //! \brief Get the reaction coefficient
        //! \return Pointer to function from X^d to X
        inline Function< Point<d, X>, X > *reaction() const
        {
            return _reaction;
        }

        //! \brief Get the forcing term
        //! \return Pointer to function from X^d to X
        inline Function< Point<d, X>, X > *forcing() const
        {
            return _forcing;
        };
    };

    /*! \brief Class for handling Diffusion-Convection-Reaction steady equation on 2D
        spaces                                                                          */
    /*! \param X Must be a type for which operations +, -, * and / are defined with
        semantics (approximately) corresponding to those of a field in a mathematical
        sense. Note that, strictly speaking, the built-in type int does not fullfil the
        requirements on a field type, since ints correspond to elements of a ring rather
        than a field, especially operation / is not the inverse of *.                   */
    template<class X>
    class DiffusionConvectionReactionEquation<2, X>
        : public Equation<2, X>
    {
        // coefficients
        Function< Point<2, X>, X >         *_diffusion;
        Function< Point<2, X>, Vector<X> > *_convection;
        Function< Point<2, X>, X >         *_reaction;
        Function< Point<2, X>, X >         *_forcing;

    public:

        typedef typename Equation<2, X>::Type Type;

        //! \brief Default constructor
        //! Construct an emty equation
        DiffusionConvectionReactionEquation()
        {
            _diffusion = 0;
            _convection = 0;
            _reaction = 0;
            _forcing = 0;
        };

        //! \brief Destructor
        ~DiffusionConvectionReactionEquation()
        {
            delete _diffusion;
            delete _convection;
            delete _reaction;
            delete _forcing;
        };

        //! \brief Get the equation type
        //! \return DIFFUSION_CONVECTION_REACTION Equation::Type
        inline Type type() const
        {
            return Equation<2, X>::DIFFUSION_CONVECTION_REACTION;
        };

        //! \brief Get the equation in Mathematical Markup Languace fformat
        //! \return QString of the equation in MathML notation
        QString mml() const
        {
            QString mml = "<mtable><mtr><mtd>";
            if(_diffusion && _convection && _reaction)
                mml += "<mo>-</mo><mo>&Del;&CenterDot;</mo><mfenced open='(' close=')'><mrow><mi"\
                       ">&mu;</mi><mo>&Del;</mo> <mi>u</mi></mrow></mfenced><mo>+</mo><mi mathva"\
                       "riant='bold'>b</mi><mo>&CenterDot;</mo><mo>&Del;</mo> <mi>u</mi><mo>+</m"\
                       "o><mi>&sigma;</mi><mi>u</mi>";
            else if(_diffusion && _convection)
                mml += "<mo>-</mo><mo>&Del;&CenterDot;</mo><mfenced open='(' close=')'><mrow><mi"\
                       ">&mu;</mi><mo>&Del;</mo> <mi>u</mi></mrow></mfenced><mo>+</mo><mi mathva"\
                       "riant='bold'>b</mi><mo>&CenterDot;</mo><mo>&Del;</mo> <mi>u</mi>";
            else if(_diffusion && _reaction)
                mml += "<mo>-</mo><mo>&Del;&CenterDot;</mo><mfenced open='(' close=')'><mrow><mi"\
                       ">&mu;</mi><mo>&Del;</mo> <mi>u</mi></mrow></mfenced><mo>+</mo><mi>&sigma"\
                       ";</mi><mi>u</mi>";
            else if(_convection && _reaction)
                mml += "<mi mathvariant='bold'>b</mi><mo>&CenterDot;</mo><mo>&Del;</mo> <mi>u</m"\
                       "i><mo>+</mo><mi>&sigma;</mi><mi>u</mi>";
            else if(_diffusion)
                mml += "<mo>-</mo><mo>&Del;&CenterDot;</mo><mfenced open='(' close=')'><mrow><mi"\
                       ">&mu;</mi><mo>&Del;</mo> <mi>u</mi></mrow></mfenced>";
            else if(_convection)
                mml += "<mi mathvariant='bold'>b</mi><mo>&CenterDot;</mo><mo>&Del;</mo> <mi>u</m"\
                       "i>";
            else if(_reaction)
                mml += "<mi>&sigma;</mi><mi>u</mi>";
            else
                mml += "<mn>0</mn>";
            if(_forcing)
                mml += "<mo>=</mo><mi>f</mi></mtd></mtr>";
            else
                mml += "<mo>=</mo><mn>0</mn></mtd></mtr>";
            if(_diffusion || _convection || _reaction || _forcing)
            {
                mml += "<mtr><mtd><mtable><mtr>";
                if(_diffusion)
                    mml += "<mtd><mi>&mu;</mi> <mfenced open='(' close=')' separators=','> <mi> "\
                           "x </mi> <mi> y </mi> </mfenced> <mo>=</mo>"
                           + _diffusion->mml()
                    + "</mtd>";
                if(_convection)
                    mml += "<mtd><mi mathvariant='bold'>b</mi> <mfenced open='(' close=')' separ"\
                           "ators=','> <mi> x </mi> <mi> y </mi> </mfenced> <mo>=</mo>"
                           + _convection->mml()
                    + "</mtd>";
                if(_reaction)
                    mml += "<mtd><mi>&sigma;</mi> <mfenced open='(' close=')' separators=','> <m"\
                           "i> x </mi> <mi> y </mi> </mfenced> <mo>=</mo>"
                           + _reaction->mml()
                    + "</mtd>";
                if(_forcing)
                    mml += "<mtd><mi>f</mi> <mfenced open='(' close=')' separators=','> <mi> x <"\
                           "/mi> <mi> y </mi> </mfenced> <mo>=</mo>"
                           + _forcing->mml()
                    + "</mtd>";
                mml += "</mtr></mtable></mtd></mtr>";
            }
            mml += "</mtable>";
            return mml;
        };

        //! \brief Set the diffusion coefficient
        //! \param diffusion Pointer to function from X^d to X
        inline void setDiffusion(Function< Point<2, X>, X > *diffusion)
        {
            delete _diffusion;
            _diffusion = diffusion;
        };

        //! \brief Set the convection coefficient
        //! \param convection Pointer to function from X^d to X
        inline void setConvection(Function< Point<2, X>, Vector<X> > *convection)
        {
            delete _convection;
            _convection = convection;
        };

        //! \brief Set the reaction coefficient
        //! \param reaction Pointer to function from X^d to X
        inline void setReaction(Function< Point<2, X>, X > *reaction)
        {
            delete _reaction;
            _reaction = reaction;
        };

        //! \brief Set the forcing term
        //! \param forcing Pointer to function from X^d to X
        inline void setForcing(Function< Point<2, X>, X > *forcing)
        {
            delete _forcing;
            _forcing = forcing;
        };

        //! \brief Get the diffusion coefficient
        //! \return Pointer to function from X^d to X
        inline Function< Point<2, X>, X > *diffusion() const
        {
            return _diffusion;
        };

        //! \brief Get the convection coefficient
        //! \return Pointer to function from X^d to X
        inline Function< Point<2, X>, Vector<X> > *convection() const
        {
            return _convection;
        };

        //! \brief Get the reaction coefficient
        //! \return Pointer to function from X^d to X
        inline Function< Point<2, X>, X > *reaction() const
        {
            return _reaction;
        }

        //! \brief Get the forcing term
        //! \return Pointer to function from X^d to X
        inline Function< Point<2, X>, X > *forcing() const
        {
            return _forcing;
        };

    };
};

#endif // DIFFUSIONCONVECTIONREACTIONEQUATION_HPP
