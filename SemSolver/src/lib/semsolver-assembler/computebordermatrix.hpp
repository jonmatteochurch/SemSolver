#ifndef COMPUTEBORDERMATRIX_HPP
#define COMPUTEBORDERMATRIX_HPP

#include <SemSolver/semspace.hpp>
#include <SemSolver/boundaryconditions.hpp>
#include <SemSolver/matrix.hpp>
#include <SemSolver/multiindex.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Assembler namespace
    /*! This namespace provides algorithms for the constuction of the algebraic matrices
        and vectors associated to the discretized problem from geometric and functional
        information stored in a SemProblem. */
    namespace Assembler
    {
        /*! Compute the boundary matrix for a 2D elliptic problem in a Spectral
            Element Space using penality method */
        //! The computed matrix is stored in the Matrix referenced by matrix
        template<class X>
        void compute_border_matrix(const SemSpace<2, X> &space,
                                   const BoundaryConditions<2, X> *boundary_conditions,
                                   const Function< Point<2, X>, X > *diffusion,
                                   double const &penality,
                                   Matrix<X> &matrix )
        {
            unsigned n = space.nodes();
            int N = space.degree();
            unsigned Mb = space.borders();

            matrix = Matrix<X>(n,n,0.);
            for(unsigned i=0; i<Mb; ++i)
            {
                for(int j=0; j<=N; ++j)
                {
                    MultiIndex<2> mi;
                    mi.setSubIndex(0,i+1);
                    mi.setSubIndex(1,j);
                    int I = space.borderIndex(mi);
                    int border = space.borderId(space.border(i));
                    if(boundary_conditions->borderType(border) ==
                       BoundaryConditions<2,X>::DIRICHLET)
                    {
                        X const &alpha = space.borderWeight(mi);
                        X const &eta = penality;
                        matrix[I][I] += alpha * eta;
                    }
                    else if(boundary_conditions->borderType(border)==
                            BoundaryConditions<2,X>::ROBIN)
                    {
                        X const &alpha = space.borderWeight(mi);
                        Point<2,X> const &x = space.borderNode(mi).point();
                        X mi = diffusion ? diffusion->evaluate(x) : 0;
                        X gamma = boundary_conditions->robinCoefficient(
                                border)->evaluate(x);
                        matrix[I][I] += alpha * mi * gamma;
                    }
                }
            }
        };
    };
};

#endif // COMPUTECONVECTIONMATRIX_HPP
