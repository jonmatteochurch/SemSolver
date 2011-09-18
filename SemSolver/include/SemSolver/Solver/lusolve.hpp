#ifndef LUSOLVER_HPP
#define LUSOLVER_HPP

#if defined _WIN32 || defined _WIN64
#	include <SemSolver/math_defines>
#endif

#include <jama_lu.h>

#include <SemSolver/matrix.hpp>
#include <SemSolver/vector.hpp>

namespace SemSolver
{
    //! Solver namespace
    /*! This namespace provides algorithms for solving an algebraic system A*x=b */
    namespace Solver
    {
        //! Solve the algebraic system A*x=b with LU factorization method
        //! \param A must be a non singular matrix
        //! \param b constant term
        //! \param x Vector reference to the computed solution
        template<class X>
        bool lu_solve(Matrix<X> const &A,
                      Vector<X> const &b,
                      Vector<X> &x)
        {
            JAMA::LU<X> lu(A);
#ifdef SEMDEBUG
            if(!lu.isNonsingular())
            {
                qWarning("SemSolver::Solver::lu_solve - ERROR : Matrix A is singular.");
                return false;
            }
#endif
            x = lu.solve(b);
            return true;
        };
    };
};

#endif // LUSOLVER_HPP
