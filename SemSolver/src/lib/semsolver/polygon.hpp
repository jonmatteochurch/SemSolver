#ifndef POLYGON_HPP
#define POLYGON_HPP

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling polygons
    //! \param d Dimension of the space
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<int d, class X>
    class Polygon;
};

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Polygon_2.h>

#include <SemSolver/point.hpp>

//! \brief Project main namespace
namespace SemSolver
{

    //! \brief Class for handling 2D polygons
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<class X>
    class Polygon<2, X>
        : public CGAL::Polygon_2< CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
    {

        //! \brief Base CGAL point class definition
        typedef CGAL::Point_2< CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
                CGAL_point;

        //! \brief Base CGAL polygon class definition
        typedef CGAL::Polygon_2< CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
                CGAL_polygon;

    public:

        //! \brief Default constructor
        inline Polygon();

        //! \brief Construct Polygon from a Point's sequence
        //! \param first Pointer to the first Point of the sequence
        //! \param last  Pointer to the last Point of the sequence
        inline Polygon(CGAL_point const *first,
                       CGAL_point const *last);

        //! \brief Test if a point lies on the Polygon
        //! \param  point The Point to test with
        //! \return The test result
        inline bool contains(Point<2, X> const &point) const;

    };

};

template<class X>
inline SemSolver::Polygon<2, X>::Polygon()
    : CGAL_polygon()
{};

template<class X>
inline SemSolver::Polygon<2, X>::Polygon(const CGAL_point *first,
                                         const CGAL_point *last)
                                             : CGAL_polygon(first, last)
{};

template<class X>
inline bool SemSolver::Polygon<2, X>::contains(const Point<2, X> &point) const
{
    return bounded_side(point) != CGAL::ON_UNBOUNDED_SIDE;
};

#endif // POLYGON_HPP
