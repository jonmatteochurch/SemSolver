#ifndef VECTOR_HPP
#define VECTOR_HPP

namespace SemSolver
{
    //! \brief Class for handling mathematical vectors
    template<class X>
    class Vector;
};

#include <tnt_array1d.h>

#include <SemSolver/point.hpp>

namespace SemSolver
{
    template<class X>
    class Vector
        : public TNT::Array1D<X>
    {
        typedef TNT::Array1D<X> TNT_array_1d;
    public:
        //! Construct empty vector
        Vector() : TNT_array_1d() {};

        //! Construct vector from TNT::Array1D
        Vector(TNT_array_1d const &vector) : TNT_array_1d(vector) {};

        //! Construct vector of dimension elements
        Vector(int dimension) : TNT_array_1d(dimension) {};
        Vector(int dimension, X const &value) : TNT_array_1d(dimension, value) {};

        //! Construct vector from point coordinates
        template<int d>
        Vector(Point<d, X> const &point)
            : TNT_array_1d(d)
        {
            for(int i=0; i<d; ++i)
                (*this)[i] = point.cartesian(i);
        }

        //! Get vector dimension
        inline int rows() const
        {
            return this->dim1();
        };
    };

    //! Vector summation
    template<class X>
    Vector<X> operator +(Vector<X> const &vec1, Vector<X> const &vec2)
    {
        if(vec1.dim() != vec2.dim())
            qFatal("");
        return vec1+vec2;
    };

    //! Vector multiplication for a scalar
    template<class X>
    X scalar(Vector<X> const &vec1, Vector<X> const &vec2)
    {
        if(vec1.dim() != vec2.dim())
            qFatal("SemSolver::Vector::scalar - ERROR : vec1 and vec2 must have the same"\
                   " dimension.");
        X product = 0;
        for(int i=0; i<vec1.dim(); ++i)
            product += vec1[i]*vec2[i];
        return product;
    };
};

#endif // VECTOR_HPP
