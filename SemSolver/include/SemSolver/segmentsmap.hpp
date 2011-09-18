#ifndef SEGMENTSVECTOR_HPP
#define SEGMENTSVECTOR_HPP

namespace SemSolver
{
    template<int d, class X>
    class SegmentsMap;
};

#include <map>

#include <SemSolver/segment.hpp>
#include <SemSolver/point.hpp>

//! \brief Class for handling maps with int as KeyType and Point as MappedType
//! \param d Dimension of the space
/*! \param X Must be a type for which operations +, -, * and / are defined
             with semantics (approximately) corresponding to those of a
             field in a mathematical sense. Note that, strictly speaking,
             the built-in type int does not fullfil the requirements on a
             field type, since ints correspond to elements of a ring rather
             than a field, especially operation / is not the inverse of * */
namespace SemSolver
{
    template<int d, class X>
    class SegmentsMap
        : std::map< int, Segment<d,X> >
    {
    private:
        typedef std::map< int, Segment<d,X> > Base;

    protected:
        typedef typename Base::iterator Iterator;

    public:
        typedef typename Base::const_iterator ConstIterator;
        using Base::begin;
        using Base::end;

    private:
        int _current_id;

    public:

        //! Default constructor
        SegmentsMap()
        {
            _current_id = 0;
        }

        //! Check if map contains a segment
        bool contains(Segment<d,X> const &segment) const
        {
            for(ConstIterator it=Base::begin(); it!=Base::end(); ++it)
                if(it->second==segment)
                    return true;
            return false;
        }

        //! Check id an id is already used
        bool has(int const &segment_id) const
        {
            for(ConstIterator it=begin(); it!=end(); ++it)
                if(it->first==segment_id)
                    return true;
            return false;
        }

        //! Get first segment in the map whith specified source
        Segment<d,X> segmentFrom(Point<d,X> const &source) const
        {
            for(ConstIterator it=begin(); it!=end(); ++it)
                if(it->second.source()==source)
                    return it->second;
            return Segment<d,X>(source, source);
        }

        //! Get first segment in the map whith specified target
        Segment<d,X> segmentTo(Point<d,X> const &target) const
        {
            for(ConstIterator it=begin(); it!=end(); ++it)
                if(it->second.target()==target)
                    return it->second;
            return Segment<d,X>(target, target);
        }

        //! Access segment by key
        Segment<d,X> const &segment(int const &id) const
        {
            ConstIterator it = Base::find(id);
#ifdef SEMDEBUG
            if(it==Base::end())
                qFatal("SemSolver::SegmentsMap::modify - ERROR : id out of range");
#endif
            return it->second;
        };

        //! Get key corresponding to a segment
        //! \param segment Segment to find, it is assumed to exist
        int id(Segment<d,X> const &segment) const
        {
            for(ConstIterator it=begin(); it!=end(); ++it)
                if(it->second==segment)
                    return it->first;
            qFatal("SemSolver::SegmentsMap::id - ERROR : segment must exist");
            return 0;
        };

        //! Inset a segment in map
        //! \return Id associated with segment
        int add(Segment<d,X> const &segment)
        {
            int id = _new_id();
            insert(id,segment);
            return id;
        }

        //! Insert a id-segment pair in map If id already exists do nothing
        void insert(int const &id, Segment<d,X> const &segment)
        {
            Base::insert(typename Base::value_type(id,segment));
        }

        //! Remove entry with specified id if exists.
        void remove(int const &id)
        {
            Base::erase(id);
        }

        //! Modify segment with specified id
        //! \param id The key to find, it is assumed to exist in map
        //! \param segment Value to be assigned
        inline void modify(int const &id, Segment<d,X> const &segment)
        {
            Iterator it = Base::find(id);
#ifdef SEMDEBUG
            if(it==Base::end())
                qFatal("SemSolver::SegmentsMap::modify - ERROR : id out of range");
#endif
            it->second = segment;
        }

        //! Get size of map
        int segments() const
        {
            return Base::size();
        };

        //! Check if there is a segment in map intesecting specified segment
        virtual bool intersect(Segment<d,X> const &segment) const
        {
            for(ConstIterator it=begin(); it!=end(); ++it)
                if(segment.intersect(it->second))
                    return true;
            return false;
        }

        /*! Check if there is a segment in map intesecting specified segment not in a
         endpoint */
        virtual bool intersectInteriorly(Segment<d,X> const &segment) const
        {
            for(ConstIterator it=begin(); it!=end(); ++it)
                if(segment.intersectInteriorly(it->second))
                    return true;
            return false;
        }

        /*! Check if segment has consisten orientation with segments in map ie if there
            are no two segments with the same target nor source */
        bool isConsistentWith(Segment<d,X> const &segment) const
        {
            for (ConstIterator it=begin(); it!=end(); ++it)
                if(it->second.source()==segment.source() ||
                   it->second.target()==segment.target() )
                    return false;
            return true;
        }

        //! Check if given point lies on a segment of the map or not
        bool haveOn(Point<d,X> const &point) const
        {
            for (ConstIterator it=begin(); it!=end(); ++it)
                if(it->second.has_on(point))
                    return true;
            return false;
        }


    private:
        int const &_new_id()
        {
            return ++_current_id;
        };

    };
};

#endif // SEGMENTSVECTOR_HPP
