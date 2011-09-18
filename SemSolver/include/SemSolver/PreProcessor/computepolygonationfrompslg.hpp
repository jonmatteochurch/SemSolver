#ifndef COMPUTEPOLYGONATIONFROMPSLG_HPP
#define COMPUTEPOLYGONATIONFROMPSLG_HPP

#ifdef __GNUC__
#  ifdef __GLIBC__
#    if ((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 1)))
#      include <stdint.h>
#    endif
#  endif
#endif

#if defined _WIN32 || defined _WIN64
#  undef max
#  include <limits>
#endif

#include <boost/shared_ptr.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Straight_skeleton_2.h>
#include <CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>
#include <CGAL/squared_distance_2.h>
#include <SemSolver/pointsset.hpp>
#include <SemSolver/semparameters.hpp>
#include <SemSolver/segment.hpp>
#include <SemSolver/polygonation.hpp>
#include <SemSolver/polygonwithholes.hpp>
#include <SemSolver/pslg.hpp>
#include <SemSolver/PreProcessor/computepolygonwithholesfrompslg.hpp>

namespace SemSolver
{
    //! \brief PreProcessor namespace
    /*! This namespace provides algorithms for the constuction of the geometric structures
        associated to the problem from geometric information stored in PSLG format. */
    namespace PreProcessor
    {
        /*! Compute a polygonation of a 2D geoemetry defined as a Planar Straight Line
            Graph */
        template<class X>
        bool compute_polygonation_from_pslg(const PSLG<X> &pslg,
                                            Polygonation<2,X> &polygonation)
        {
            typedef CGAL::Straight_skeleton_2< CGAL::Filtered_kernel<
                    CGAL::Simple_cartesian<X> > > Skeleton;
            typedef typename boost::shared_ptr< Skeleton > SkeletonPtr;

            typedef typename Skeleton::Vertex_iterator VertexIterator;
            typedef typename Skeleton::Face_iterator FaceIterator;
            typedef typename Skeleton::Halfedge_handle HalfedgeHandle;
            typedef typename Skeleton::Halfedge_iterator HalfedgeIterator;
            typedef typename Segment<2,X>::less Segment_less;
            typedef std::map<Segment<2,X>, int, Segment_less> SegmentIdsMap;
            typedef typename SegmentIdsMap::value_type Pair;
            PolygonWithHoles<2,X> domain;
            if(!compute_polygon_with_holes_from_pslg(pslg, domain))
            {
                return false;
            }
            SkeletonPtr skeleton = CGAL::create_interior_straight_skeleton_2(domain.cgal());

            polygonation.clear();

            SegmentIdsMap face_id;
            int index = 0;
            for (FaceIterator it = skeleton->faces_begin();
            it!=skeleton->faces_end();++it)
                face_id.insert(Pair(
                        Segment<2, X>(it->halfedge()->vertex()->point(),
                                      it->halfedge()->opposite()->vertex()->point()),
                        ++index));
            SegmentIdsMap boundary_id;
            index=0;
            for (HalfedgeIterator it = skeleton->halfedges_begin();
            it!= skeleton->halfedges_end(); ++it)
                if(it->is_border())
                    boundary_id.insert(Pair(
                            Segment<2, X>(it->vertex()->point(),
                                          it->opposite()->vertex()->point()), ++index));
            for (FaceIterator it = skeleton->faces_begin();
            it != skeleton->faces_end(); ++it)
            {
                Polygon<2, X> polygon;
                std::vector<int> neighbours;
                HalfedgeHandle halfedge = it->halfedge();
                do
                {
                    polygon.push_back(halfedge->vertex()->point());
                    if(halfedge->is_bisector())
                        neighbours.push_back(face_id[
                                Segment<2, X>(halfedge->opposite()->face()->halfedge()->vertex()->point(),
                                              halfedge->opposite()->face()->halfedge()->opposite()->vertex()->point())]);
                    else
                        neighbours.push_back(-boundary_id[
                                Segment<2, X>(halfedge->opposite()->vertex()->point(),
                                              halfedge->opposite()->opposite()->vertex()->point())]);
                    halfedge = halfedge->next();
                }
                while(halfedge->vertex()->point()!=it->halfedge()->vertex()->point());
                polygonation.addElement(polygon, neighbours);
            }
            return true;
        }
    }
}

#endif // COMPUTEPOLYGONATIONFROMPSLG_HPP
