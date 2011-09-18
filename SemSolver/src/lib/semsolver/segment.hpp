#ifndef SEGMENT_HPP
#define SEGMENT_HPP

namespace SemSolver
{
    template<int d, class X>
    class Segment;
};

#include <CGAL/Cartesian.h>
#include <CGAL/Object.h>
#include <CGAL/Segment_2.h>
#include <CGAL/intersections.h>

#include <SemSolver/point.hpp>

//! \brief The SemSolver namespace
namespace SemSolver
{
    template<class X>
    class Segment<2,X>
        : public CGAL::Segment_2< CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
    {
        typedef CGAL::Point_2< CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
                CGAL_point;
        typedef CGAL::Segment_2< CGAL::Filtered_kernel< CGAL::Simple_cartesian<X> > >
                CGAL_segment;

    public:

        Segment(CGAL_segment const &s) : CGAL_segment(s) {};

        //! Construct an oriented segment from source point to target point
        Segment(Point<2,X> const &source, Point<2,X> const &target)
            : CGAL_segment(source, target) {};

        CGAL_segment const &cgal() const
        {
            return *this;
        };

        //! Check if two segments intersect
        bool intersect(Segment<2,X> const &segment) const
        {
            return CGAL::do_intersect(segment, *this);
        };

        //! Check if two segment intersect themselves not in their endpoints
        bool intersectInteriorly(Segment<2,X> const &segment) const
        {
            if(!intersect(segment))
                return false;
            CGAL_point intersection;
            CGAL::Object result = CGAL::intersection(segment.cgal(), this->cgal());
            if(!CGAL::assign(intersection, result))
                return true;
            if(intersection!=segment.source() && intersection!=segment.target() &&
               intersection!=this->source() && intersection!=this->target())
                return true;
            return false;
        };

        struct less
        {
            bool operator() (Segment const &s0, Segment const &s1) const
            {
                if(s0.source().x()<s1.source().x())
                    return true;
                if(s0.source().x()>s1.source().x())
                    return false;
                if(s0.source().y()<s1.source().y())
                    return true;
                if(s0.source().y()>s1.source().y())
                    return false;
                if(s0.target().x()<s1.target().x())
                    return true;
                if(s0.target().x()>s1.target().x())
                    return false;
                if(s0.target().y()<s1.target().y())
                    return true;
                //if(s0.target().y()>s1.target().y())
                return false;
            }
        };
    };
};

#endif // SEGMENT_HPP
