#ifndef COMPUTEDIFFUSIONMATRIX_HPP
#define COMPUTEDIFFUSIONMATRIX_HPP

#include <SemSolver/function.hpp>
#include <SemSolver/point.hpp>
#include <SemSolver/matrix.hpp>
#include <SemSolver/semspace.hpp>

namespace SemSolver
{
    //! \brief Assembler namespace
    /*! This namespace provides algorithms for the constuction of the algebraic matrices
        and vectors associated to the discretized problem from geometric and functional
        information stored in a SemProblem. */
    namespace Assembler
    {
        /*! Compute the diffusion matrix for a 2D elliptic problem in a Spectral
            Element Space  */
        //! The computed matrix is stored in the Matrix referenced by matrix
        template<class X>
        void compute_diffusion_matrix(const SemSpace<2, X> &space,
                                      const Function< Point<2, X>, X> *diffusion,
                                      Matrix<X> &matrix)
        {
            typedef typename SemSpace<2, X>::Node Node;

            int n = space.nodes();
            int N = space.degree();

            matrix = Matrix<X>(n,n,0.);

            for (int I0=0; I0<n; ++I0)
            {
                Node const &node0 = space.node(I0);
                for (int I1=0; I1<n; ++I1)
                {
                    Node const &node1 = space.node(I1);
                    int l0=0, l1=0;
                    while(l0<node0.supportSubDomains() && l1<node1.supportSubDomains())
                    {
                        if(node0.subDomainIndex(l0).subIndex(0) < node1.subDomainIndex(l1).subIndex(0))
                            ++l0;
                        else if(node0.subDomainIndex(l0).subIndex(0) > node1.subDomainIndex(l1).subIndex(0))
                            ++l1;
                        else
                        {
                            int i = node0.subDomainIndex(l0).subIndex(0);
                            for (int j2=0; j2<=N; ++j2)
                            {
                                for (int k2=0; k2<=N; ++k2)
                                {
                                    MultiIndex<3> mi2;
                                    mi2.setSubIndex(0,i);
                                    mi2.setSubIndex(1,j2);
                                    mi2.setSubIndex(2,k2);
                                    X alpha = space.subDomainWeight(mi2);
                                    Point<2,X> x2 = space.subDomainNode(mi2).point();
                                    X mi = diffusion->evaluate(x2);
                                    Vector<X> const &grad0 = space.baseFunction(I0)->evaluateRestrictionGradient(i,x2);
                                    Vector<X> const &grad1 = space.baseFunction(I1)->evaluateRestrictionGradient(i,x2);
                                    matrix[I0][I1] += alpha * mi * scalar(grad1,grad0);
                                }
                            }
                            ++l0;
                            ++l1;
                        }
                    }
                }
            }
        };
    };
};

#endif // COMPUTEDIFFUSIONMATRIX_HPP
