#ifndef POLYNOMIALFUNCTION_HPP
#define POLYNOMIALFUNCTION_HPP

#include <SemSolver/polynomial.hpp>
#include <SemSolver/function.hpp>
#include <SemSolver/point.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling polynomial separable functions
    /*! The image of a point X of dimension d is the product of d polynomial evaluated at
        each coordinate of X, i.e. for d=3 F(X) = f1(x1)*f2(x2)*f3(x3)                  */
    //! \param d Dimension of the space
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<int d, class X>
    class PolynomialFunction : public Function< Point<d,X>, X >
    {
    private:
        Polynomial<X> _polynomials[d];

    public:
        //! \brief Default Constructor
        PolynomialFunction()
        {
            for(int i=0; i<d; ++i)
                _polynomials[i] = 0.;
        };

        //! \brief Construct Polynomial function from polynomials
        PolynomialFunction(Polynomial<X> const *polynomials)
        {
            for(int i=0; i<d; ++i)
                _polynomials[i] = polynomials[i];
        };

        //! \brief Copy constructor
        PolynomialFunction(PolynomialFunction<d,X> const &polynomial_function)
        {
            for(int i=0; i<d; ++i)
            {
                _polynomials[i] = polynomial_function.polynomial(i);
            }
        };

        //! \brief Destructor
        ~PolynomialFunction()
        {};

        //! Assigment operator
        PolynomialFunction<d,X> &operator=(PolynomialFunction<d,X> const &polynomial_function)
                                          {
            for(int i=0; i<d; ++i)
                _polynomials[i] = polynomial_function.polynomial(i);
            return *this;
        };

        //! Access polynomial projection over a component
        //! \param index The index of the conmpont on wich to project
        Polynomial<X> const &polynomial(int const &index) const
        {
#ifdef SEMDEBUG
            if(index<0 || index>=d)
                qFatal("SemSolver::PolynomialFunction::polynomial - ERROR : index out of"\
                       "range");
#endif
            return _polynomials[index];
        };

        //! Set function projection at index
        void setPolynomial(int const &index, Polynomial<X> const &poly)
        {
#ifdef SEMDEBUG
            if(index<0 || index>=d)
                qFatal("SemSolver::PolynomialFunction::setPolynomial ERROR : index out o"\
                       "f range");
#endif
            _polynomials[index] = poly;
        };

        //! \brief Compute function value at Point x
        X evaluate(Point<d,X> const &x) const
        {
            double result = 1;
            for(int i=0; i<d; ++i)
                result *= polynomial(i)(x.cartesian(i));
            return result;
        };
    };
};

#endif // POLYNOMIALFUNCTION_HPP
