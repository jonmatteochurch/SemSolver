#ifndef MATRIX_HPP
#define MATRIX_HPP

namespace SemSolver
{
    template<class X>
    class Matrix;
};

#include <tnt_array2d.h>
#include <jama_eig.h>

#include <SemSolver/vector.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling mathematical matrices
    template<class X>
    class Matrix
        : public TNT::Array2D<X>
    {
        typedef TNT::Array2D<X> Base;

    public:
        Matrix();

        Matrix(int rows, int columns);

        Matrix(int rows, int columns, X const &value);

        inline int rows() const;

        inline int columns() const;

        template<class Y>
        friend Matrix<Y> operator+(Matrix<Y> const &, Matrix<Y> const &);

        Vector<X> realEigenvalues() const;
    };

    template<class X>
    Matrix<X> operator *(Matrix<X> const &mat1, Matrix<X> const &mat2);

    template<class X>
    Matrix<X> operator +(Matrix<X> const &mat1, Matrix<X> const &mat2);
};


//! \brief Construct an empty (0x0) matrix
template<class X>
SemSolver::Matrix<X>::Matrix()
    : Base()
{
};

//! \brief Construct a matrix of a specific size
//! \param rows Number of rows
//! \param columns Number of columns
template<class X>
SemSolver::Matrix<X>::Matrix(int rows, int columns)
    : Base(rows, columns)
{
};

//! \brief Construct a matrix with specific size and equal entries
//! \param rows Number of rows
//! \param columns Number of columns
//! \param value to assign to matrix entries
template<class X>
SemSolver::Matrix<X>::Matrix(int rows, int columns, X const &value)
    : Base(rows, columns, value)
{
};

//! \brief Get the number of rows
//! \return Rows number
template<class X>
inline int SemSolver::Matrix<X>::rows() const
{
    return Base::dim1();
};

//! \brief Get the number of columns
//! \return Columns number
template<class X>
inline int SemSolver::Matrix<X>::columns() const
{
    return Base::dim2();
};

//! \brief Get the eigenvalues
//! \return Eigenvalues vector
template<class X>
SemSolver::Vector<X> SemSolver::Matrix<X>::realEigenvalues() const
{
    Vector<X> eigenvalues;
    JAMA::Eigenvalue<double> eig(*this);
    eig.getRealEigenvalues(eigenvalues);
    return eigenvalues;
};

//! \brief Matrix multiplication
//! \param mat1 First matrix
//! \param mat2 Second matrix
//! \return Product matrix
template<class X>
SemSolver::Matrix<X> SemSolver::operator *(Matrix<X> const &mat1,
                                           Matrix<X> const &mat2)
{
    int r1 = mat1.dim1();
    int c1 = mat1.dim2();
    int r2 = mat2.dim1();
    int c2 = mat2.dim2();
#ifdef SEMDEBUG
    if(c1 != r2)
        qFatal("SemSolver::operator * - ERROR : the number of columns of mat1 must match"\
               " the number of rows of mat2.");
#endif
    Matrix<X> product(r1, c2,0);
    for (int i=0; i<r1; i++)
        for (int j=0; j<c2; j++)
            for (int k=0; k<c1; k++)
                product += mat1[i][k] * mat2[k][j];
    return product;
};

//! \brief Matrix summation
//! \param mat1 First matrix
//! \param mat2 Second matrix
//! \return Sum matrix
template<class X>
SemSolver::Matrix<X> SemSolver::operator +(Matrix<X> const &mat1,
                                           Matrix<X> const &mat2)
{
#ifdef SEMDEBUG
    int r1 = mat1.dim1();
    int c1 = mat1.dim2();
    int r2 = mat2.dim1();
    int c2 = mat2.dim2();
    if(c1 != c2)
        qFatal("SemSolver::operator * - ERROR : the number of columns of mat1 must match"\
               "the number of columns of mat2.");
    if(r1 != r2)
        qFatal("SemSolver::Matrix::operator * - ERROR : the number of rows of mat1 must "\
               "match the number of the rows of mat2.");
#endif
    return mat1+mat2;
};

#endif // MATRIX_HPP
