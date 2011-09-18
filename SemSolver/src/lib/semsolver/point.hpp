#ifndef POINT_HPP
#define POINT_HPP

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling euclidean points
    //! \param d Dimension of the space
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template <int d, class X>
    class Point;
};

#include <cmath>

#include <CGAL/Cartesian.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/centroid.h>

//! \brief Project main namespace
namespace SemSolver
{

    //! \brief Class for handling 2D euclidean points
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<class X>
    class Point<2,X>
        : public CGAL::Point_2< CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
    {

        //! \brief Base CGAL point class definition
        typedef CGAL::Point_2< CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
                CGAL_Point;

    public:

        //! \brief Default constructor
        inline Point();

        //! \brief Construct Point from CGAL_Point
        inline Point(CGAL_Point const &cgal_point);

        //! \brief Construct a Point with given coordinates
        //! \param x Abscissa
        //! \param y Ordinate
        inline Point(X const &x,
                     X const &y);
    };

};

template<class X>
inline SemSolver::Point<2, X>::Point()
    : CGAL_Point()
{};

template<class X>
inline SemSolver::Point<2, X>::Point(const CGAL_Point &cgal_point)
    : CGAL_Point(cgal_point)
{};

template<class X>
inline SemSolver::Point<2, X>::Point(const X &x,
                                     const X &y)
    : CGAL_Point(x, y)
{};

#endif // POINT_HPP
