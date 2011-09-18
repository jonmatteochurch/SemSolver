#ifndef PROBLEM_HPP
#define PROBLEM_HPP

namespace SemSolver
{
    template<int d, class X>
    class Problem;
}

#include <SemSolver/semgeometry.hpp>
#include <SemSolver/equation.hpp>
#include <SemSolver/boundaryconditions.hpp>
#include <SemSolver/semparameters.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    /*! \brief Class for handling a mathematical problem given by an equation, boundary
        conditions, geometry and parameters                                             */
    template<int d, class X>
    class Problem
    {
        const SemGeometry<d, X>        *_geometry;
        const Equation<d, X>           *_equation;
        const BoundaryConditions<d, X> *_boundary_conditions;
        const SemParameters<X>         *_parameters;

    public:
        Problem();
        ~Problem();
        inline void setGeometry(const SemGeometry<d, X> *geometry);
        inline void setEquation(const Equation<d, X> *equation);
        inline void setBoundaryConditions(
                const BoundaryConditions<d, X> *boundary_conditions);
        inline void setParameters(const SemParameters<X> *parameters);

        inline void clearGeometry();
        inline void clearEquation();
        inline void clearBoundaryConditions();
        inline void clearParameters();

        inline const SemGeometry<d, X> *geometry() const;
        inline const Equation<d, X> *equation() const;
        inline const BoundaryConditions<d, X> *boundaryConditions() const;
        inline const SemParameters<X> *parameters() const;

        inline bool isDefined();
    };
};

//! Default constructor
template<int d, class X>
SemSolver::Problem<d, X>::Problem()
{
    _geometry = 0;
    _equation = 0;
    _boundary_conditions = 0;
    _parameters = 0;
};

//! Destructor
template<int d, class X>
SemSolver::Problem<d, X>::~Problem()
{
    delete _geometry;
    delete _equation;
    delete _boundary_conditions;
    delete _parameters;
};

//! Set the geometry on wich the problem is defined
template<int d, class X>
inline void SemSolver::Problem<d, X>::setGeometry(const SemGeometry<d, X> *geometry)
{
    _geometry = geometry;
};

//! Set the equation describing the problem
template<int d, class X>
inline void SemSolver::Problem<d, X>::setEquation(const Equation<d, X> *equation)
{
    _equation = equation;
};

//! Set the boundary conditions for the problem
template<int d, class X>
inline void SemSolver::Problem<d, X>::setBoundaryConditions(
        const BoundaryConditions<d, X> *boundary_conditions)
{
    _boundary_conditions = boundary_conditions;
};

//! Set the parameters for the problem
template<int d, class X>
inline void SemSolver::Problem<d, X>::setParameters(const SemParameters<X> *parameters)
{
    _parameters = parameters;
};

//! Clear problem geometry
template<int d, class X>
inline void SemSolver::Problem<d, X>::clearGeometry()
{
    delete _geometry;
    _geometry = 0;
};

//! Clear problem equation
template<int d, class X>
inline void SemSolver::Problem<d, X>::clearEquation()
{
    delete _equation;
    _equation = 0;
};

//! Clear problem boundary conditions
template<int d, class X>
inline void SemSolver::Problem<d, X>::clearBoundaryConditions()
{
    delete _boundary_conditions;
    _boundary_conditions = 0;
};

//! Clear problem parameters
template<int d, class X>
inline void SemSolver::Problem<d, X>::clearParameters()
{
    delete _parameters;
    _parameters = 0;
};

//! Access problem geometry
template<int d, class X>
inline const SemSolver::SemGeometry<d, X> *SemSolver::Problem<d, X>::geometry() const
{
    return _geometry;
};

//! Access problem equation
template<int d, class X>
inline const SemSolver::Equation<d, X> *SemSolver::Problem<d, X>::equation() const
{
    return _equation;
};

//! Access problem boundary conditions
template<int d, class X>
inline const SemSolver::BoundaryConditions<d, X> *
        SemSolver::Problem<d, X>::boundaryConditions() const
{
    return _boundary_conditions;
};

//! Access problem parameters
template<int d, class X>
inline const SemSolver::SemParameters<X> *SemSolver::Problem<d, X>::parameters() const
{
    return _parameters;
};

/*! Check if problem is well defined i.e. if geometry, equation, boundary conditions
    and parameters are defined for the problem */
template<int d, class X>
inline bool SemSolver::Problem<d, X>::isDefined()
{
    return ( _geometry && _equation && _boundary_conditions && _parameters );
};

#endif // PROBLEM_HPP
