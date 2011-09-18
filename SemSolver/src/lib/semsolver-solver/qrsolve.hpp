#ifndef QRSOLVE_HPP
#define QRSOLVE_HPP

#if defined _WIN32 || defined _WIN64
#	include <SemSolver/math_defines>
#endif

#include <jama_qr.h>

#include <SemSolver/matrix.hpp>
#include <SemSolver/vector.hpp>

namespace SemSolver
{
    //! Solver namespace
    /*! This namespace provides algorithms for solving an algebraic system A*x=b */
    namespace Solver
    {
        //! Solve the algebraic system A*x=b with QR factorization method
        //! \param A must be a full rank matrix
        //! \param b constant term
        //! \param x Vector reference to the computed solution
        template<class X>
        bool qr_solve(Matrix<X> const &A,
                      Vector<X> const &b,
                      Vector<X> &x)
        {
            JAMA::QR<double> qr(A);
#ifdef SEMDEBUG
            if(!qr.isFullRank())
            {
                qWarning("SemSolver::Solver::qr_solve - ERROR : Matrix A is not full ran"\
                         "k.");
                return false;
            }
#endif
            x = qr.solve(b);
            return true;
        };
    };
};

#endif // QRSOLVE_HPP
