#ifndef POINTSBIMAP_HPP
#define POINTSBIMAP_HPP

namespace SemSolver
{
    template<int d, class X>
    class PointsBimap;
};

#include <SemSolver/point.hpp>
#include <SemSolver/segment.hpp>
#include <SemSolver/pointsmap.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling bi-directional maps between Points and Integers
    //! \param d Dimension of the space
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<int d, class X>
    class PointsBimap;

    //! \brief Class for handling bi-directional maps between 2D Points and Integers
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<class X>
    class PointsBimap<2, X>
        : private PointsMap<2, X, int>
    {
        typedef PointsMap<2, X, int> Base;
        typedef typename Base::Iterator Iterator;

    public:
        typedef typename Base::KeyType KeyType;
        typedef typename Base::MappedType MappedType;
        typedef typename Base::ValueType ValueType;
        typedef typename Base::ConstIterator ConstIterator;
        typedef typename Base::SizeType SizeType;

    private:
        int last_id;

        inline int new_id();

    public:

        inline PointsBimap(const X &tolerance);

        Iterator insert(const ValueType &x);

        inline MappedType insertPoint(const KeyType &x);

        inline const KeyType &point(const MappedType &y) const;

        inline const MappedType &id(const KeyType &x) const;

        inline void modifyPoint(const MappedType &y, const KeyType &x);

        inline void modifyId(const KeyType &x, const MappedType &y);

        inline void erasePoint(const KeyType &x);

        inline void eraseId(const MappedType &y);

        ConstIterator findPoint(const KeyType &x) const;

        Iterator findPoint(const KeyType &x);

        ConstIterator findId(const MappedType &y) const;

        Iterator findId(const MappedType &y);

        inline bool hasPoint(const KeyType &x) const;

        bool hasId(const MappedType &y) const;

        bool hasPointOn(const Segment<2, X> &s) const;

        using Base::begin;
        using Base::clear;
        using Base::has;
        using Base::isEmpty;
        using Base::end;
        using Base::erase;
        using Base::size;
    };
};

template<class X>
inline int SemSolver::PointsBimap<2, X>::new_id()
{
    return ++last_id;
};

//! \brief Constructor
/*! \param tol Points whose distance is below this value are treated as
    the same point                                                              */
template<class X>
inline SemSolver::PointsBimap<2, X>::PointsBimap(const X &tol)
    : Base(tol), last_id(0)
{
};

//! \brief Insert a point-id value
template<class X>
typename SemSolver::PointsBimap<2, X>::Iterator
        SemSolver::PointsBimap<2, X>::insert(const ValueType &x)
{
    if(!hasId(x.second))
        return Base::insert(x);
    return Base::end();
};

//! \brief Insert a point if it doesn't exists otherwise do nothing
//! \param x the point to be inserted
//! \return the id of the point inserted
template<class X>
inline typename SemSolver::PointsBimap<2, X>::MappedType
        SemSolver::PointsBimap<2, X>::insertPoint(const KeyType &x)
{
    return Base::insert(ValueType(x,new_id()))->second;
};

//! \brief Get a point
//! \param y The id of the point to be find. It must exist
//! \return The point corresponding to the specified id
template<class X>
const typename SemSolver::PointsBimap<2, X>::KeyType &
        SemSolver::PointsBimap<2, X>::point(const MappedType &y) const
{
    ConstIterator it = begin();
    while(it!=end())
    {
        if(it->second == y)
            break;
        else
            ++it;
    }
#ifdef SEMDEBUG
    if(it==end())
        qFatal("SemSolver::PointsBimap::point: no mapped value y.");
#endif
    return it->first;
};

//! \brief Get an id
//! \param x The point to be find. It must exist
//! \return The id corresponding to the specified point
template<class X>
const typename SemSolver::PointsBimap<2, X>::MappedType &
        SemSolver::PointsBimap<2, X>::id(const KeyType &x) const
{
    ConstIterator it = find(x);
#ifdef SEMDEBUG
    if(it==end())
        qFatal("SemSolver::PointsBimap::point: no key value x");
#endif
    return it->second;
};

//! Modify a point
//! \param y The id corresponding to the point to be modified. It must exist
//! \param x The new point value
template<class X>
inline void SemSolver::PointsBimap<2, X>::modifyPoint(const MappedType &y,
                                                      const KeyType &x)
{
#ifdef SEMDEBUG
    if(findPoint(x)!=end())
        qFatal("SemSolver:PointsBimap<2, X>::modifyPoint - ERROR : key value x must be u"\
               "nique.");
#endif
    eraseId(y);
    insert(ValueType(x,y));
};

//! Modify an id
//! \param x The point corresponding to the id to be modified. It must exist
//! \param y The new id value
template<class X>
inline void SemSolver::PointsBimap<2, X>::modifyId(const KeyType &x,
                                                   const MappedType &y)
{
#ifdef SEMDEBUG
    if(findId(y)!=end())
        qFatal("SemSolver:PointsBimap<2, X>::modifyId - ERROR : mapped value y must be u"\
               "nique.");
#endif
    erasePoint(x);
    insert(ValueType(x,y));
};

//! \brief Erase a point-id entry if exists
//! \param x The point to be removed
template<class X>
inline void SemSolver::PointsBimap<2, X>::erasePoint(const KeyType &x)
{
    Iterator it = findPoint(x);
    if(it!=end())
        erase(it);
};

//! \brief Erase a point-id entry if exists
//! \param y The id to be removed
template<class X>
inline void SemSolver::PointsBimap<2, X>::eraseId(const MappedType &y)
{
    Iterator it = findId(y);
    if(it!=end())
        erase(it);
};

//! \brief Find a point-id entry by its point value
//! \param x The point value
//! \return ConstIterator to the entry if found, to end otherwise
template<class X>
inline typename SemSolver::PointsBimap<2, X>::ConstIterator
        SemSolver::PointsBimap<2, X>::findPoint(const KeyType &x) const
{
    return Base::find(x);
};

//! \brief Find a point-id entry by its point value
//! \param x The point value
//! \return Iterator to the entry if found, to end otherwise
template<class X>
inline typename SemSolver::PointsBimap<2, X>::Iterator
        SemSolver::PointsBimap<2, X>::findPoint(const KeyType &x)
{
    return Base::find(x);
};

//! \brief Find a point-id entry by its id value
//! \param id The id value
//! \return ConstIterator to the entry if found, to end otherwise
template<class X>
typename SemSolver::PointsBimap<2, X>::ConstIterator
        SemSolver::PointsBimap<2, X>::findId(const MappedType &id) const
{
    ConstIterator it = begin();
    while(it != end())
    {
        if(it->second == id)
            break;
        else
            ++it;
    }
    return it;
};

//! \brief Find a point-id entry by its id value
//! \param id The id value
//! \return ConstIterator to the entry if found, to end otherwise
template<class X>
typename SemSolver::PointsBimap<2, X>::Iterator
        SemSolver::PointsBimap<2, X>::findId(const MappedType &id)
{
    Iterator it = begin();
    while(it != end())
    {
        if(it->second == id)
            break;
        else
            ++it;
    }
    return it;
};

//! \brief Check if an entry exists
//! \param x The point to be found
//! \return Whether the point exists or not
template<class X>
inline bool SemSolver::PointsBimap<2, X>::hasPoint(const KeyType &x) const
{
    return findPoint(x)!=end();
};

//! \brief Check if an entry exists
//! \param y The id to be found
//! \return Whether the id exists or not
template<class X>
bool SemSolver::PointsBimap<2, X>::hasId(const MappedType &y) const
{
    return findId(y)!=end();
};

//! \brief Check if there is a point lying on a given segment
//! \param segment The segment to be checked
//! \return Whether there exists such a point or not
template<class X>
inline bool SemSolver::PointsBimap<2, X>::hasPointOn(const Segment<2, X> &segment) const
{
    for (ConstIterator it=begin(); it!=end(); ++it)
        if(it->first!=segment.source() && it->first!=segment.target() && segment.has_on(it->first))
            return true;
    return false;
};

#endif // POINTSBIMAP_HPP
