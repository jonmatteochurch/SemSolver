#ifndef COMPUTEALGEBRAICSYSTEM_HPP
#define COMPUTEALGEBRAICSYSTEM_HPP

#include <SemSolver/semspace.hpp>
#include <SemSolver/problem.hpp>
#include <SemSolver/diffusionconvectionreactionequation.hpp>
#include <SemSolver/matrix.hpp>
#include <SemSolver/vector.hpp>

#include <SemSolver/Assembler/computediffusionmatrix.hpp>
#include <SemSolver/Assembler/computeconvectionmatrix.hpp>
#include <SemSolver/Assembler/computereactionmatrix.hpp>
#include <SemSolver/Assembler/computebordermatrix.hpp>
#include <SemSolver/Assembler/computeforcingvector.hpp>
#include <SemSolver/Assembler/computebordervector.hpp>

namespace SemSolver
{
    //! \brief Assembler namespace
    /*! This namespace provides algorithms for the constuction of the algebraic matrices
        and vectors associated to the discretized problem from geometric and functional
        information stored in a SemProblem. */
    namespace Assembler
    {
        /*! Compute the algebraic system A * u = f associated to a 2D elliptic problem in
            a Spectral Element Space */
        //! The computed system is stored in the Matrix and vectored refereced by A and f
        template<class X>
        void compute_algebraic_system(const SemSpace<2, X> &space,
                                      const Problem<2, X> &problem,
                                      Matrix<X> &A,
                                      Vector<X> &f)
        {
            if( problem.equation()->type()==Equation<2, X>::DIFFUSION_CONVECTION_REACTION )
            {
                const DiffusionConvectionReactionEquation<2, X> *equation =
                        (const DiffusionConvectionReactionEquation<2, X> *)problem.equation();
                int n = space.nodes();
                A = Matrix<X>(n,n,0.);
                f = Vector<X>(n,0.);
                Matrix<X> Ad, Ac, Ar, Ab;
                Vector<X> ff, fb;
                if(equation->diffusion())
                {
#ifdef SEMDEBUG
                    qDebug() << "diffusion matrix";
#endif
                    Assembler::compute_diffusion_matrix(space, equation->diffusion(), Ad);
                    A += Ad;
                }
                if(equation->convection())
                {
#ifdef SEMDEBUG
                    qDebug() << "convection matrix";
#endif
                    Assembler::compute_convection_matrix(space,equation->convection(),Ac);
                    A += Ac;
                }
                if(equation->reaction())
                {
#ifdef SEMDEBUG
                    qDebug() << "reaction matrix";
#endif
                    Assembler::compute_reaction_matrix(space, equation->reaction(), Ar);
                    A += Ar;
                }
#ifdef SEMDEBUG
                qDebug() << "border matrix";
#endif
                Assembler::compute_border_matrix(space,
                                                 problem.boundaryConditions(),
                                                 equation->diffusion(),
                                                 problem.parameters()->penality(),
                                                 Ab);
                A += Ab;
                if(equation->forcing())
                {
#ifdef SEMDEBUG
                    qDebug() << "forcing vector";
#endif
                    Assembler::compute_forcing_vector(space, equation->forcing(), ff);
                    f += ff;
                }
#ifdef SEMDEBUG
                qDebug() << "border vector";
#endif
                Assembler::compute_border_vector(space,
                                                 problem.boundaryConditions(),
                                                 equation->diffusion(),
                                                 problem.parameters()->penality(),
                                                 fb);
                f += fb;
            }
        };
    };
};

#endif // COMPUTEALGEBRAICSYSTEM_HPP
