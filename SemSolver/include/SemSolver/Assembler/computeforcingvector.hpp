#ifndef COMPUTEFORCINGVECTOR_HPP
#define COMPUTEFORCINGVECTOR_HPP

#include <SemSolver/problem.hpp>
#include <SemSolver/vector.hpp>

namespace SemSolver
{
    //! \brief Assembler namespace
    /*! This namespace provides algorithms for the constuction of the algebraic matrices
        and vectors associated to the discretized problem from geometric and functional
        information stored in a SemProblem. */
    namespace Assembler
    {
        /*! Compute the forcing vector for a 2D elliptic problem in a Spectral
            Element Space */
        //! The computed matrix is stored in the Matrix referenced by matrix
        template<class X>
        void compute_forcing_vector(const SemSpace<2, X> &space,
                                    const Function< Point<2, X>, X > *forcing,
                                    Vector<X> &vector)
        {
            typedef typename SemSpace<2,X>::Node Node;

            int n = space.nodes();
            vector = Vector<X>(n,0.);
            for(int I=0; I<n; ++I)
            {
                Node const &node = space.node(I);
                for(int l=0; l<node.supportSubDomains(); ++l)
                {
                    MultiIndex<3> Il = node.subDomainIndex(l);
                    X const &alpha = space.subDomainWeight(Il);
                    Point<2,X> const &x = space.subDomainNode(Il).point();
                    X f = forcing->evaluate(x);
                    vector[I] += alpha * f;
                }
            }
        };
    };
};

#endif // COMPUTEFORCINGVECTOR_HPP
