#ifndef BUILDSOLUTION_HPP
#define BUILDSOLUTION_HPP

#include <SemSolver/function.hpp>
#include <SemSolver/semspace.hpp>
#include <SemSolver/vector.hpp>

namespace SemSolver
{
    namespace PostProcessor
    {
        template<class X>
        void build_solution(const SemSpace<2, X> &space,
                            const Vector<X> &coefficients,
                            Function< Point<2, X>, X > *&solution)
        {
            typedef typename SemSpace<2, X>::Element Element;
            delete solution;
            solution = new Element(&space, coefficients);
        };
    };
};

#endif // BUILDSOLUTION_HPP
