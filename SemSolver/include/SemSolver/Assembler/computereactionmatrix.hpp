#ifndef COMPUTEREACTIONMATRIX_HPP
#define COMPUTEREACTIONMATRIX_HPP

#include <SemSolver/semspace.hpp>
#include <SemSolver/matrix.hpp>

namespace SemSolver
{
    //! \brief Assembler namespace
    /*! This namespace provides algorithms for the constuction of the algebraic matrices
        and vectors associated to the discretized problem from geometric and functional
        information stored in a SemProblem. */
    namespace Assembler
    {
        /*! Compute the reaction matrix for a 2D elliptic problem in a Spectral
            Element Space */
        //! The computed matrix is stored in the Matrix referenced by matrix
        template<class X>
        void compute_reaction_matrix(const SemSpace<2, X> &space,
                                     Function< Point<2, X>, X> const *reaction,
                                     Matrix<double> &matrix)
        {
            typedef typename SemSpace<2,X>::Node Node;

            int n = space.nodes();
            matrix = Matrix<X>(n,n,0.);
            for(int I0=0; I0<n; ++I0)
            {
                Node const &node0 = space.node(I0);
                Point<2,X> x = node0.point();
                for(int l=0; l<node0.supportSubDomains(); ++l)
                {
                    MultiIndex<3> mi1 = node0.subDomainIndex(l);
                    int I1 = space.subDomainIndex(mi1);
                    X alpha = space.subDomainWeight(mi1);
                    X gamma = reaction->evaluate(x);
                    matrix[I0][I1] += alpha * gamma;
                }
            }
        };
    };
};

#endif // COMPUTEREACTIONMATRIX_HPP
