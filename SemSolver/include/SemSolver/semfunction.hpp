#ifndef SEMFUNCTION_HPP
#define SEMFUNCTION_HPP

#include <SemSolver/function.hpp>
#include <SemSolver/polynomialfunction.hpp>
#include <SemSolver/bilineartransformation.hpp>
#include <SemSolver/point.hpp>
#include <SemSolver/semgeometry.hpp>
#include <SemSolver/matrix.hpp>
#include <SemSolver/vector.hpp>
#include <SemSolver/semparameters.hpp>

namespace SemSolver
{
    //! Class for handling spectral elements functions
    /*! For each element of the partition of the domain geometry there are defined a
    tranformation to a canonical element, and a polynomial function */
    template <int d,class X>
            class SemFunction
                : public Function< Point<d,X>, X >
    {};

    //! Class for handling 2D spectral elements functions
    /*! For each element of the partition of the domain geometry there are defined a
    tranformation to a canonical element, and a polynomial function */
    template<class X>
    class SemFunction<2,X>
        : public Function< Point<2,X>, X >
    {
        typedef std::vector< PolynomialFunction<2,X> > PolynomialFunctionVector;
        typedef std::vector< BilinearTransformation<X> > BilinearTransformationsVector;

        Polygonation<2,X> _polygonation;
        PolynomialFunctionVector _polynomials;
        BilinearTransformationsVector _maps;

    public:
        //***** tranformation to be generalized *****//
                /*! Construct a spectral element function on a SemGeometry from a
                PloynomialFunction vector and a BilinearTranformation vector */
                SemFunction(
                        SemGeometry<2,X> const &geometry,
                        PolynomialFunctionVector const &polynomials,
                        BilinearTransformationsVector const &maps)
                            : _polygonation(geometry.subDomains()),
                            _polynomials(polynomials),
                            _maps(maps)
        {};

        //! Copy constructor
        SemFunction(SemFunction const &sem_function)
            : _polygonation(sem_function._polygonation),
            _polynomials(sem_function._polynomials),
            _maps(sem_function._maps)
        {};

        //! Access polynomial transformed restriction on a subdomain element
        inline PolynomialFunction<2,X> const &polynomial(const unsigned &index) const
        {
#if SEMDEBUG
            if(index>=_polygonation.size())
                qFatal("SemSolver::SemFunction::polynomial error : index out of range");
#endif
            return _polynomials.at(index);
        };

        //! Set polynomial transformed restriction component on a subdomain element
        inline void setPolynomialComponent(const unsigned &index,
                                                  const unsigned &component,
                                                  const Polynomial<X> &poly)
        {
#if SEMDEBUG
            if(index>=_polygonation.size())
                qFatal("SemSolver::SemFunction::polynomial error : index out of range");
#endif
            _polynomials.at(index).setPolynomial(component, poly);
        };

        //! Access transformation from a subdomain element to canonical element
        inline BilinearTransformation<X> const &map(const unsigned &index) const
        {
#if SEMDEBUG
            if(index>=_polygonation.size())
                qFatal("SemSolver::SemFunction::polynomial error : index out of range");
#endif
            return _maps.at(index);
        };

        //! Compute function value at a point
        double evaluate(Point<2,X> const &x) const
        {
            std::vector<unsigned> element_index = _polygonation.elementIndicesAt(x);
            if(element_index.size()==0)
                return 0.;
            Point<2,X> x_hat = map(element_index[0]).evaluateInverse(x);
            return polynomial(element_index[0]).evaluate(x_hat);
        };

        //! Compute gradient of function restriction on a subdomain element
        Vector<X> evaluateRestrictionGradient(
                int const &element_index,
                Point<2,X> const &P) const
        {
            Vector<double> gradient(2);
            Point<2,X> const &P_hat = map(element_index).evaluateInverse(P);
            Matrix<X> tIJ_phi = map(element_index).evaluateTransposeInverseJacobian(P_hat);
            X const &x_hat = P_hat.x();
            X const &y_hat = P_hat.y();
            Polynomial<X> const &px = polynomial(element_index).polynomial(0);
            Polynomial<X> const &py = polynomial(element_index).polynomial(1);
            X psi_x = px.derivative()(x_hat) * py(y_hat);
            X psi_y = px(x_hat) * py.derivative()(y_hat);
            gradient[0] = psi_x * tIJ_phi[0][0] + psi_y * tIJ_phi[0][1];
            gradient[1] = psi_x * tIJ_phi[1][0] + psi_y * tIJ_phi[1][1];
            return gradient;
        };
    };
};

#endif // SEMFUNCTION_HPP
