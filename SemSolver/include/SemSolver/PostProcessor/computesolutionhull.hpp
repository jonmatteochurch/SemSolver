#ifndef COMPUTESOLUTIONHULL_HPP
#define COMPUTESOLUTIONHULL_HPP

#include <SemSolver/vector.hpp>
#include <SemSolver/semspace.hpp>

namespace SemSolver
{
    namespace PostProcessor
    {
        template<class X>
        void compute_solution_hull(const SemSpace<2, X> &space,
                                   const Vector<X> &coefficients,
                                   double &xmin,
                                   double &ymin,
                                   double &zmin,
                                   double &xmax,
                                   double &ymax,
                                   double &zmax)
        {
            if(!space.nodes())
                return;
            if(!coefficients.rows())
                return;
            xmin = xmax = space.node(0).point().x();
            ymin = ymax = space.node(0).point().y();
            zmin = zmax = coefficients[0];
            for(unsigned i=1; i<space.nodes(); ++i)
            {
                if(space.node(i).point().x() < xmin)
                    xmin = space.node(i).point().x();
                if(space.node(i).point().x() > xmax)
                    xmax = space.node(i).point().x();
                if(space.node(i).point().y() < ymin)
                    ymin = space.node(i).point().y();
                if(space.node(i).point().y() > ymax)
                    ymax = space.node(i).point().y();
            }
            for(int i=1; i<coefficients.rows(); ++i)
            {
                /********* Not precise **********/
                if(coefficients[i] < zmin)
                    zmin = coefficients[i];
                if(coefficients[i] > zmax)
                    zmax = coefficients[i];
            }
        };
    };
};

#endif // COMPUTESOLUTIONHULL_HPP
