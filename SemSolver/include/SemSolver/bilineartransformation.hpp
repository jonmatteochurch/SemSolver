#ifndef BILINEARTRANSFORMATION_HPP
#define BILINEARTRANSFORMATION_HPP

namespace SemSolver
{
    template<class X>
    class BilinearTransformation;
};

#include <cmath>

#include <SemSolver/homeomorphism.hpp>
#include <SemSolver/matrix.hpp>
#include <SemSolver/point.hpp>
#include <SemSolver/polygon.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class representing a bilinear tranformation of 2D euclidean space
    /*! \param X Must be a type for which operations +, -, * and / are defined with
        semantics (approximately) corresponding to those of a field in a mathematical
        sense. Note that, strictly speaking, the built-in type int does not fullfil the
        requirements on a field type, since ints correspond to elements of a ring rather
        than a field, especially operation / is not the inverse of *                    */
    template<class X>
    class BilinearTransformation
        : public Homeomorphism< Point<2,X>, Point<2,X> >
    {
        // domain
        Polygon<2,X> _omega;

        // coefficients
        X _Px, _Qx, _Rx, _Sx;
        X _Py, _Qy, _Ry, _Sy;
        X _PQ, _PR, _PS, _QR, _QS, _RS;

        // tolerance for inverse evaluation
        X _tolerance;

    public:

        BilinearTransformation();

        BilinearTransformation(Point<2,X> const &A,
                               Point<2,X> const &B,
                               Point<2,X> const &C,
                               Point<2,X> const &D);

        BilinearTransformation(Polygon<2,X> const &omega,
                               X const &tolerance);

        ~BilinearTransformation();

        void setOmega(Polygon<2,X> const &omega);

        void setTolerance(X const &tolerance);

        inline Polygon<2,X> const &omega() const;

        inline X const &tolerance() const;

        Point<2,X> evaluate(Point<2,X> const &point) const;

        Point<2,X> evaluateInverse(Point<2,X> const &point) const;

        inline X evaluateJacobianDeterminant(Point<2,X> const &point) const;

        Matrix<double>
                evaluateTransposeInverseJacobian(Point<2,X> const &point) const;
    };
};

//! \brief Construct the trivial transformation
template<class X>
SemSolver::BilinearTransformation<X>::BilinearTransformation()
{
    _Px =  0., _Py =  0.;
    _Qx = -1., _Qy =  0.;
    _Rx =  0., _Ry = -1.;
    _Sx =  0., _Sy =  0.;
    _PQ =  0., _PR =  0.;
    _PS =  0., _QR =  1.;
    _QS =  0., _RS =  0.;
    _tolerance = 0;
};

/*! \brief Construct the bilinear transformation that maps the quadrangle
    \f$\Omega=ABCD\f$ into the cononical square \f$\hat\Omega = (-1., 1.)^2\f$.         */
/*! It maps \f$A\f$ into \f$(-1., -1.)\f$, \f$B\f$ into \f$(1., -1.)\f$, \f$C\f$ into
    \f$(1., 1.)\f$, and \f$D\f$ into \f$(-1., 1.)\f$.                                   */
template<class X>
SemSolver::BilinearTransformation<X>::BilinearTransformation(Point<2,X> const &A,
                                                             Point<2,X> const &B,
                                                             Point<2,X> const &C,
                                                             Point<2,X> const &D)
{
    _Px = (A.x()+B.x()+C.x()+D.x())/4.;
    _Py = (A.y()+B.y()+C.y()+D.y())/4.;
    _Qx = (A.x()-B.x()-C.x()+D.x())/4.;
    _Qy = (A.y()-B.y()-C.y()+D.y())/4.;
    _Rx = (A.x()+B.x()-C.x()-D.x())/4.;
    _Ry = (A.y()+B.y()-C.y()-D.y())/4.;
    _Sx = (A.x()-B.x()+C.x()-D.x())/4.;
    _Sy = (A.y()-B.y()+C.y()-D.y())/4.;

    _PQ = _Px*_Qy - _Py*_Qx;
    _PR = _Px*_Ry - _Py*_Rx;
    _PS = _Px*_Sy - _Py*_Sx;
    _QR = _Qx*_Ry - _Qy*_Rx;
    _QS = _Qx*_Sy - _Qy*_Sx;
    _RS = _Rx*_Sy - _Ry*_Sx;

    _tolerance = 0.;
};

/*! \brief Construct the bilinear transformation that maps the quadrangle
    \f$\Omega = ABCD\f$ into the cononical square \f$\hat\Omega = (-1., 1.)^2\f$.       */
/*! It maps \f$A\f$ into \f$(-1., -1.)\f$, \f$B\f$ into \f$(1., -1.)\f$, \f$C\f$ into
    \f$(1., 1.)\f$, and \f$D\f$ into \f$(-1., 1.)\f$.                                   */
/*! \param omega The polygon to be mapped onto \f$\hat\Omega\f$. It must be a simple
    convex counterclockwise-oriented quadrangle                                         */
/*! \param tolerance The value bolow which numbers are treates as zero. It must be non
    negative                                                                            */
template<class X>
SemSolver::BilinearTransformation<X>::BilinearTransformation(Polygon<2,X> const &omega,
                                                             X const &tolerance)
{
    setOmega(omega);
    setTolerance(tolerance);
};

//! \brief Destructor
template<class X>
SemSolver::BilinearTransformation<X>::~BilinearTransformation()
{
}

/*! \brief Set the quadrangle \f$Omega\f$ to be mapped onto the canonical square        */
/*! \param omega The polygon to be mapped onto \f$\hat\Omega\f$. It must be a simple
    convex counterclockwise-oriented quadrangle                                         */
template<class X>
void SemSolver::BilinearTransformation<X>::setOmega(Polygon<2,X> const &omega)
{
#ifdef SEMDEBUG
    if(omega.size()!=4)
        qFatal("SemSolver::BilinearTransformation::setOmega - ERROR : omega must be a qu"\
               "adrangle.");
    if(!omega.is_simple())
        qFatal("SemSolver::BilinearTransformation::setOmega - ERROR : omega must be simp"\
               "le.");
    if(!omega.is_convex())
        qFatal("SemSolver::BilinearTransformation::setOmega - ERROR : omega must be conv"\
               "ex.");
    if(!omega.is_counterclockwise_oriented())
        qFatal("SemSolver::BilinearTransformation::setOmega - ERROR : omega must be coun"\
               "terclockwise oriented.");
#endif // SEMDEBUG
    _omega = omega;

    Point<2,X> const &A = omega.vertex(0);
    Point<2,X> const &B = omega.vertex(1);
    Point<2,X> const &C = omega.vertex(2);
    Point<2,X> const &D = omega.vertex(3);

    _Px = (A.x()+B.x()+C.x()+D.x())/4.;
    _Py = (A.y()+B.y()+C.y()+D.y())/4.;
    _Qx = (A.x()-B.x()-C.x()+D.x())/4.;
    _Qy = (A.y()-B.y()-C.y()+D.y())/4.;
    _Rx = (A.x()+B.x()-C.x()-D.x())/4.;
    _Ry = (A.y()+B.y()-C.y()-D.y())/4.;
    _Sx = (A.x()-B.x()+C.x()-D.x())/4.;
    _Sy = (A.y()-B.y()+C.y()-D.y())/4.;

    _PQ = _Px*_Qy - _Py*_Qx;
    _PR = _Px*_Ry - _Py*_Rx;
    _PS = _Px*_Sy - _Py*_Sx;
    _QR = _Qx*_Ry - _Qy*_Rx;
    _QS = _Qx*_Sy - _Qy*_Sx;
    _RS = _Rx*_Sy - _Ry*_Sx;
};

/*! \brief Set the tolerance used in evaluating the inverse tranformation               */
/*! \param tolerance The value bolow which numbers are treates as zero. It must be non
    negative                                                                            */
template<class X>
void SemSolver::BilinearTransformation<X>::setTolerance(X const &tolerance)
{
#ifdef SEMDEBUG
    if(tolerance<0)
        qFatal("SemSolver::BilinearTransformation::setTolerance - ERROR : tolerance must"\
               "be non negative.");
#endif // SEMDEBUG
    _tolerance = tolerance;
};

//! \brief Get the reference to \f$\Omega\f$
//! \return The quadrangle transformed onto \f$\hat\Omega\f$
template<class X>
inline SemSolver::Polygon<2,X> const &SemSolver::BilinearTransformation<X>::omega() const
{
    return _omega;
}

/*! \brief Get the tolerance used in evaluating the inverse tranformation               */
//! \return Reference to the tolerance
template<class X>
inline X const &SemSolver::BilinearTransformation<X>::tolerance() const
{
    return _tolerance;
};

//! \brief Evaluate the direct tranformation at a point
//! \param point The point to be mapped
template<class X>
SemSolver::Point<2,X>
        SemSolver::BilinearTransformation<X>::evaluate(Point<2,X> const &point) const
{
    X x = _Px - _Qx*point.x() - _Rx*point.y() + _Sx*point.x()*point.y();
    X y = _Py - _Qy*point.x() - _Ry*point.y() + _Sy*point.x()*point.y();
    return Point<2,X>(x,y);
};

//! \brief Evaluate the inverse tranformation at a point
/*! It uses different algorithms if \f$\Omega\f$ is a parallelogram a trapezoid or a more
    generic quadrangle.                                                                 */
//! \param point The point to be mapped
template<class X>
SemSolver::Point<2,X>
        SemSolver::BilinearTransformation<X>::evaluateInverse(
                Point<2,X> const &point) const
{
    X x, y;
    X XQ = point.x()*_Qy - point.y()*_Qx;
    X XR = point.x()*_Ry - point.y()*_Rx;
    X XS = point.x()*_Sy - point.y()*_Sx;
    if( std::abs(_QS)>_tolerance && std::abs(_RS)>_tolerance )
    {
        // Omega is not a parallelogram nor a trapezoid
        X psi = pow(XS-_PS+_QR, 2.) + 2.*(XR*_QS+XQ*_RS-XS*_QR) - \
                4.*_PQ*_RS;
        X n0 = XS-_PS;
        X n1 = _QR - sqrt(psi);
        X d0 = 2.*_QS;
        X d1 = 2.*_RS;
        x = -(n0-n1)/d0;
        y = -(n0+n1)/d1;
    }
    else if(std::abs(_RS)>_tolerance) // QS trascurabile
    {
        // Omega is a vertical trapezoid
        x = _Qx*(XR-_PR)/(_Sx*(XQ-_PQ)-_Qx*_QR);
        y = (XQ-_PQ)/_QR;
    }
    else if(std::abs(_QS)>_tolerance)
    {
        // Omega is a horizontal trapezoid
        x = (_PR-XR)/_QR;
        y = _Rx*(_PQ-XQ)/(_Sx*(_PR-XR)-_Rx*_QR);
    }
    else
    {
        // Omega is a parallelogram
        x = (_PR-XR)/_QR;
        y = (XQ-_PQ)/_QR;
    }
    return Point<2,X>(x,y);
};

//! \brief Evaluate the Jacobian determinant at a point
//! \param point The point where to evaluate the Jacobian
template<class X>
inline X SemSolver::BilinearTransformation<X>::evaluateJacobianDeterminant(
        Point<2,X> const &point) const
{
    return _QR - point.x()*_QS + point.y()*_RS;
};

//! \brief Evaluate the Transpose Inverse of the Jacobian matrix
//! \param point The point where to evaluate the Jacobian
template<class X>
SemSolver::Matrix<double>
        SemSolver::BilinearTransformation<X>::evaluateTransposeInverseJacobian(
                Point<2,X> const &point) const
{
    Matrix<double> tiJ(2,2,1/evaluateJacobianDeterminant(point));
    tiJ[0][0] *= point.x()*_Sy - _Ry;
    tiJ[0][1] *= _Rx - point.x()*_Sx;
    tiJ[1][0] *= _Qy - point.y()*_Sy;
    tiJ[1][1] *= point.y()*_Sx - _Qx;
    return tiJ;
};

#endif // BILINEARTRANSFORMATION_HPP
