#ifndef POLYGONWITHHOLES_HPP
#define POLYGONWITHHOLES_HPP

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling polygons with holes
    //! \param d Dimension of the space
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template <int d, class X>
            class PolygonWithHoles;
}

#include <CGAL/Cartesian.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Polygon_with_holes_2.h>

#include <SemSolver/polygon.hpp>

//! \brief Project main namespace
namespace SemSolver
{

    //! \brief Class for handling 2D polygons with holes
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<class X>
    class PolygonWithHoles<2,X>
        : public CGAL::Polygon_with_holes_2<
        CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
    {
        //! \brief Base CGAL polygon with holes class definition
        typedef CGAL::Polygon_with_holes_2<
                CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
                CGAL_Polygon_with_holes;
    public:
        inline CGAL_Polygon_with_holes const &cgal()
        {
            return *this;
        };
    };
};

#endif // POLYGONWITHHOLES_HPP
