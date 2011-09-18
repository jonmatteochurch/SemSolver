#ifndef CHOLESKYSOLVE_HPP
#define CHOLESKYSOLVE_HPP

#if defined _WIN32 || defined _WIN64
#	include <SemSolver/math_defines>
#endif

#include <jama_cholesky.h>

#include <SemSolver/matrix.hpp>
#include <SemSolver/vector.hpp>

namespace SemSolver
{
    //! Solver namespace
    /*! This namespace provides algorithms for solving an algebraic system A*x=b */
    namespace Solver
    {
        //! Solve the algebraic system A*x=b with Cholesky method
        //! \param A must be a SPD matrix
        //! \param b constant term
        //! \param x Vector reference to the computed solution
        template<class X>
        bool cholesky_solve(Matrix<X> const &A,
                            Vector<X> const &b,
                            Vector<X> &x)
        {
            JAMA::Cholesky<X> cholesky(A);
#ifdef SEMDEBUG
            if(!cholesky.is_spd())
            {
                qWarning("SemSolver::Solver::cholesky_solve - ERROR : Matrix A is not a "\
                         "symmetric, positive definite matrix.");
                return false;
            }
#endif
            x = cholesky.solve(b);
            return true;
        };
    };
};

#endif // CHOLESKYSOLVE_HPP
