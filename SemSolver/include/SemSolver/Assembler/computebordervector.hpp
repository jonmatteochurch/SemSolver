#ifndef COMPUTEBORDERVECTOR_HPP
#define COMPUTEBORDERVECTOR_HPP

#include <SemSolver/semspace.hpp>
#include <SemSolver/boundaryconditions.hpp>
#include <SemSolver/vector.hpp>

namespace SemSolver
{
    //! \brief Assembler namespace
    /*! This namespace provides algorithms for the constuction of the algebraic matrices
        and vectors associated to the discretized problem from geometric and functional
        information stored in a SemProblem. */
    namespace Assembler
    {
        /*! Compute the boundary vector for a 2D elliptic problem in a Spectral
            Element Space using penality method */
        //! The computed matrix is stored in the Matrix referenced by matrix
        template<class X>
        void compute_border_vector(const SemSpace<2, X> &space,
                                   const BoundaryConditions<2, X> *boundary_conditions,
                                   const Function< Point<2, X>, X > *diffusion,
                                   const double &penality,
                                   Vector<X> &vector)
        {
            int n = space.nodes();
            int N = space.degree();
            int Mb = space.borders();
            vector = Vector<X>(n,0.);
            for(int i=0; i<Mb; ++i)
            {
                for(int j=0; j<=N; ++j)
                {
                    MultiIndex<2> mi;
                    mi.setSubIndex(0,i+1);
                    mi.setSubIndex(1,j);
                    int I = space.borderIndex(mi);
                    X alpha = space.borderWeight(mi);
                    int border = space.borderId(space.border(i));
                    Point<2,X> const &x = space.borderNode(mi).point();
                    if(boundary_conditions->borderType(border) == BoundaryConditions<2,X>::DIRICHLET)
                    {
                        X const &eta = penality;
                        X g = boundary_conditions->dirichletData(border)->evaluate(x);
                        vector[I] += alpha * eta * g;
                    }
                    else if(boundary_conditions->borderType(border)==BoundaryConditions<2,X>::NEUMANN)
                    {
                        X mi = diffusion ? diffusion->evaluate(x) : 0;
                        X h = boundary_conditions->neumannData(border)->evaluate(x);
                        vector[I]+= alpha * mi * h;
                    }
                    else if(boundary_conditions->borderType(border)==BoundaryConditions<2,X>::ROBIN)
                    {
                        X mi = diffusion ? diffusion->evaluate(x) : 0;
                        X r = boundary_conditions->robinData(border)->evaluate(x);
                        vector[I]+= alpha * mi * r;
                    }
                }
            }
        };
    };
};

#endif // COMPUTEBORDERVECTOR_HPP
