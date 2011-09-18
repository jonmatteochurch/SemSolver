#ifndef COMPUTECONVECTIONMATRIX_HPP
#define COMPUTECONVECTIONMATRIX_HPP

#include <SemSolver/function.hpp>
#include <SemSolver/point.hpp>
#include <SemSolver/vector.hpp>
#include <SemSolver/matrix.hpp>

namespace SemSolver
{
    //! \brief Assembler namespace
    /*! This namespace provides algorithms for the constuction of the algebraic matrices
        and vectors associated to the discretized problem from geometric and functional
        information stored in a SemProblem. */
    namespace Assembler
    {
        /*! Compute the convection matrix for a 2D elliptic problem in a Spectral
            Element Space */
        //! The computed matrix is stored in the Matrix referenced by matrix
        template<class X>
        void compute_convection_matrix(const SemSpace<2, X> &space,
                                       const Function< Point<2, X>,
                                       Vector<X> > *convection,
                                       Matrix<X> &matrix)
        {
            typedef typename SemSpace<2,X>::Node Node;

            int n = space.nodes();

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
                        if(node0.subDomainIndex(l0).subIndex(0) <
                           node1.subDomainIndex(l1).subIndex(0))
                            ++l0;
                        else if(node0.subDomainIndex(l0).subIndex(0) >
                                node1.subDomainIndex(l1).subIndex(0))
                            ++l1;
                        else
                        {
                            MultiIndex<3> mi0 = node0.subDomainIndex(l0);
                            int i = mi0.subIndex(0);
                            X alpha = space.subDomainWeight(mi0);
                            Point<2,X> x0 = node0.point();
                            Vector<X> beta = convection->evaluate(x0);
                            Vector<X> const &grad1 =
                                    space.baseFunction(I1)->\
                                    evaluateRestrictionGradient(i,x0);
                            matrix[I0][I1] += alpha * scalar(beta,grad1);
                            ++l0;
                            ++l1;
                        }
                    }
                }
            }
        };
    };
};

#endif // COMPUTECONVECTIONMATRIX_HPP
