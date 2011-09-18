#ifndef COMPUTEPOLYGONWITHHOLESFROMPSLG_HPP
#define COMPUTEPOLYGONWITHHOLESFROMPSLG_HPP

#include <SemSolver/polygonwithholes.hpp>
#include <SemSolver/pslg.hpp>
#include <SemSolver/sequenceslist.hpp>

namespace SemSolver
{
    //! \brief PreProcessor namespace
    /*! This namespace provides algorithms for the constuction of the geometric structures
        associated to the problem from geometric information stored in PSLG format. */
    namespace PreProcessor
    {
        //! Get sequences of vertices representing the set of polygons given by a PSLG
        template<class X>
        bool compute_vertices_sequences_from_pslg(PSLG<X> const &pslg,
                                                  Sequences_list &vertices_sequences)
        {
            for(unsigned i=0; i<pslg.segments(); ++i)
            {
                bool segment_added = false;
                for(Sequences_list::iterator it = vertices_sequences.begin();
                it!=vertices_sequences.end() && !segment_added;
                ++it)
                {
                    if(pslg.segment(i).source==it->back())
                    {
                        it->push_back(pslg.segment(i).target);
                        segment_added = true;
                    }
                    else if(pslg.segment(i).target==it->back())
                    {
                        it->push_back(pslg.segment(i).source);
                        segment_added = true;
                    }
                }
                if(!segment_added)
                {
                    Sequence sequence;
                    sequence.push_back(pslg.segment(i).source);
                    sequence.push_back(pslg.segment(i).target);
                    vertices_sequences.push_back(sequence);
                }
            }
            Sequences_list::iterator begin = vertices_sequences.begin();
            Sequences_list::iterator end = vertices_sequences.end();
            Sequences_list::iterator before_end = --vertices_sequences.end();
            for(Sequences_list::iterator it_0 = begin; it_0!=before_end && it_0 !=end;
                ++it_0)
            {
                bool is_closed = (it_0->front()==it_0->back());
                if(!is_closed)
                {
                    Sequences_list::iterator after_it_0 = it_0; ++after_it_0;
                    for(Sequences_list::iterator it_1 = after_it_0; it_1!=end &&
                        !is_closed; ++it_1)
                    {
                        if(it_0->back()==it_1->front())
                        {
                            for(Sequence::const_iterator it_2=it_1->begin();
                            ++it_2!=it_1->end(); )
                                it_0->push_back(*it_2);
                            vertices_sequences.erase(it_1);
                            end = vertices_sequences.end();
                            is_closed = (it_0->front()==it_0->back());
                            it_1 = it_0;
                        }
                        else if(it_0->back()==it_1->back())
                        {
                            for(Sequence::const_reverse_iterator it_2=it_1->rbegin();
                            ++it_2!=it_1->rend(); )
                                it_0->push_back(*it_2);
                            vertices_sequences.erase(it_1);
                            end = vertices_sequences.end();
                            is_closed = (it_0->front()==it_0->back());
                            it_1 = it_0;
                        }
                        else if(it_0->front()==it_1->back())
                        {
                            for(Sequence::const_reverse_iterator it_2=it_1->rbegin();
                            ++it_2!=it_1->rend(); )
                                it_0->push_front(*it_2);
                            vertices_sequences.erase(it_1);
                            end = vertices_sequences.end();
                            is_closed = (it_0->front()==it_0->back());
                            it_1 = it_0;
                        }
                        else if(it_0->front()==it_1->front())
                        {
                            for(Sequence::const_iterator it_2=it_1->begin();
                            ++it_2!=it_1->end(); )
                                it_0->push_front(*it_2);
                            vertices_sequences.erase(it_1);
                            end = vertices_sequences.end();
                            is_closed = (it_0->front()==it_0->back());
                            it_1 = it_0;
                        }
                    }
                }
                before_end = --vertices_sequences.end();
            }
            return true;
        }

        //! Compute the polygon with holes given by a PSLG
        template<class X>
        bool compute_polygon_with_holes_from_pslg(PSLG<X> const &pslg,
                                                  PolygonWithHoles<2,X> &polygon)
        {
            typedef std::list< Polygon<2,X> > Polygons_list;
            typedef typename Polygons_list::iterator Polygon_iterator;
            typedef typename Polygons_list::const_iterator Polygon_const_iterator;

            typedef std::map<int,int> Vertices_map;
            typedef typename Polygon<2,X>::Vertex_const_iterator Vertex_const_iterator;

            polygon.clear();
            Sequences_list vertices_sequences;
            compute_vertices_sequences_from_pslg(pslg,vertices_sequences);
            if(vertices_sequences.size()<1)
                return false;
            bool *is_used_vertex = new bool[pslg.vertices()];
            Vertices_map vertices_map;
            for(unsigned i=0; i<pslg.vertices(); ++i)
            {
                is_used_vertex[i] = false;
                vertices_map.insert(Vertices_map::value_type(pslg.vertex(i).number,i));
            }
            Polygons_list polygons;
            for(Sequences_list::const_iterator it_0=vertices_sequences.begin();
            it_0!=vertices_sequences.end(); ++it_0)
            {
#ifdef SEMDEBUG
                if(it_0->front()!=it_0->back())
                {
                    qWarning("PSLG::to_polygon : segments must define closed polygons");
                    return false;
                }
#endif
                Polygon<2,X> subpolygon;
                for(Sequence::const_iterator it_1=it_0->begin(); it_1!=--it_0->end(); ++it_1)
                {
#ifdef SEMDEBUG
                    if(is_used_vertex[vertices_map[*it_1]])
                    {
                        qWarning("PSLG::to_polygon : no more than two segments must pass through each vertex");
                        return false;
                    }
                    else
                    {
#endif
                        double const &x = pslg.vertex(vertices_map[*it_1]).x;
                        double const &y = pslg.vertex(vertices_map[*it_1]).y;
                        subpolygon.push_back(Point<2,X>(x,y));
                        is_used_vertex[vertices_map[*it_1]] = true;
#ifdef SEMDEBUG
                    }
#endif
                }
#ifdef SEMDEBUG
                if(subpolygon.is_empty())
                {
                    qWarning("PSLG::to_polygon : segments must define non empty polygons");
                    return false;
                }
                if(!subpolygon.is_simple())
                {
                    qWarning("PSLG::to_polygon : segments must define simple polygons");
                    return false;
                }
#endif
                polygons.push_back(subpolygon);
            }
#ifdef SEMDEBUG
            for(unsigned i=0; i<pslg.vertices(); ++i)
            {
                if(!is_used_vertex[i])
                {
                    qWarning("PSLG::to_polygon : there are unused vertex");
                    return false;
                }
            }
#endif
            Polygon_iterator outer;
            bool found = false;
            for(Polygon_iterator it_0 = polygons.begin(); it_0!=polygons.end() && !found;
            ++it_0)
            {
                bool is_outer = true;
                for(Polygon_const_iterator it_1 = polygons.begin(); it_1!=polygons.end()
                    &&is_outer; ++it_1)
                    if(it_0!=it_1)
                        for (Vertex_const_iterator it_2 = it_1->vertices_begin();
                it_2 != it_1->vertices_end() &&is_outer; ++it_2)
                            if(!it_0->has_on_bounded_side(*it_2))
                                is_outer = false;
                if(is_outer)
                {
                    outer = it_0;
                    found = true;
                }
            }
            if(outer->is_clockwise_oriented())
                outer->reverse_orientation();
            polygon.outer_boundary() = *outer;
            polygons.erase(outer);
#ifdef SEMDEBUG
            if(polygons.size()<pslg.holes())
            {
                qWarning("PSLG::to_polygon : there are too many holes");
                return false;
            }
            if(polygons.size()>pslg.holes())
            {
                qWarning("PSLG::to_polygon : there are not enough holes");
                return false;
            }
#endif
            for(unsigned i=0; i<pslg.holes(); ++i)
            {
                Polygon_iterator it=polygons.begin();
                Point<2,X> hole(pslg.hole(i).x,pslg.hole(i).y);
#ifdef SEMDEBUG
                while(!it->has_on_bounded_side(hole))
                {
                    ++it;
                    if(it==polygons.end())
                    {
                        qWarning("PSLG::to_polygon : there is no hole inside inner polygon");
                        return false;
                    }
                }
#endif
                if(it->is_counterclockwise_oriented())
                    it->reverse_orientation();
                polygon.add_hole(*it);
                polygons.erase(it);
            }
            return true;
        }
    }
}

#endif // COMPUTEPOLYGONWITHHOLESFROMPSLG_HPP
