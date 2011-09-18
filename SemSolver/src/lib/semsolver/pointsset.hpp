#ifndef POINTSSET_HPP
#define POINTSSET_HPP

namespace SemSolver
{
    template<int d, class X>
    class PointsSet;
};


#include <SemSolver/point.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling sets of Points
    //! \param d Dimension of the space
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<int d, class X>
    class PointsSet;

    //! \brief Class for handling sets of 2D Points
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<class X>
    class PointsSet<2, X>
    {
        typedef QList< Point<2, X> > list;

    protected:
        typedef typename list::iterator Iterator;

    public:
        typedef Point<2, X> KeyType;
        typedef Point<2, X> MappedType;
        typedef typename list::value_type ValueType;
        typedef typename list::const_iterator ConstIterator;
        typedef typename list::size_type SizeType;

    private:
        list points;
        const double tolerance;
        bool less(const KeyType &x, const KeyType &y);
        inline bool equal(const KeyType &x, const KeyType &y);

    protected:
        inline Iterator begin();
        inline Iterator end();
        inline Iterator find(const KeyType &x);

    public:
        inline PointsSet(const X &tolerance);
        inline ConstIterator begin() const;
        inline void clear();
        inline bool has(const KeyType &x) const;
        inline bool isEmpty() const;
        inline ConstIterator end() const;
        inline void erase(Iterator position);
        inline SizeType erase(const KeyType &x);
        inline void erase(Iterator first, Iterator last);
        inline ConstIterator find(const KeyType &x) const;
        inline Iterator insert(const ValueType &x);
        Iterator insert(Iterator position, const ValueType &x);
        template<class InputIterator>
        void insert(InputIterator first, InputIterator last);
        inline SizeType size() const;
        inline MappedType &operator[](const KeyType &key);
    };
};

template<class X>
bool SemSolver::PointsSet<2, X>::less(const KeyType &x, const KeyType &y)
{
    if(x.x()+tolerance < y.x())
        return true;
    if(x.x()-tolerance > y.x())
        return false;
    if(x.y()+tolerance < y.y())
        return true;
    return false;
};

template<class X>
inline bool SemSolver::PointsSet<2, X>::equal(const KeyType &x, const KeyType &y)
{
    return !less(x,y) && !less(y,x);
};

//! \brief Constructor
/*! \param tol Points whose distance is below this value are treated as the same point  */
template<class X>
inline SemSolver::PointsSet<2, X>::PointsSet(const X &tol)
    : tolerance(tol)
{
};

template<class X>
inline typename SemSolver::PointsSet<2, X>::Iterator
        SemSolver::PointsSet<2, X>::begin()
{
    return points.begin();
};

//! \brief Get ConstIterator to first element
//! \return ConstIterator to the first element of the set
template<class X>
inline typename SemSolver::PointsSet<2, X>::ConstIterator
        SemSolver::PointsSet<2, X>::begin() const
{
    return points.begin();
};

//! \brief Clear set constent
template<class X>
inline void SemSolver::PointsSet<2, X>::clear()
{
    points.clear();
};

//! \brief Check if an entry exists
//! \param x The point to be found
//! \return Whether the entry exists or not
template<class X>
inline bool SemSolver::PointsSet<2, X>::has(const KeyType &x) const
{
    return find(x)!=end();
};

//! \brief Check if there are entries
//! \return Whether the set is empty or not
template<class X>
inline bool SemSolver::PointsSet<2, X>::isEmpty() const
{
    return points.empty();
};

template<class X>
inline typename SemSolver::PointsSet<2, X>::Iterator SemSolver::PointsSet<2, X>::end()
{
    return points.end();
};

//! \brief Get ConstIterator to past-the-end location
//! \return ConstIterator to the location succeeding the last element in a set
template<class X>
inline typename SemSolver::PointsSet<2, X>::ConstIterator
        SemSolver::PointsSet<2, X>::end() const
{
    return points.end();
};

//! \brief Erase an entry
//! \param position Iterator to the element to be removed
template<class X>
inline void SemSolver::PointsSet<2, X>::erase(Iterator position)
{
    points.erase(position);
};

//! \brief Erase entries if exist
//! \param x The point value corresponding to the entry to be removed
//! \return The number of elements erased: 1 or 0
template<class X>
inline typename SemSolver::PointsSet<2, X>::SizeType
        SemSolver::PointsSet<2, X>::erase(const KeyType &x)
{
    return points.erase(x);
};

//! \brief Erase entries in a range
//! \param first Iterator to the first entry to be removed
//! \param last Iterator to the position just beyond the last element to be removed
template<class X>
inline void SemSolver::PointsSet<2, X>::erase(Iterator first, Iterator last)
{
    points.erase(first, last);
};

//! \brief Find a point entry
//! \param x The point value to search
//! \return ConstIterator to the entry if found, to end otherwise
template<class X>
inline typename SemSolver::PointsSet<2, X>::ConstIterator
        SemSolver::PointsSet<2, X>::find(const KeyType &x) const
{
    ConstIterator it = begin();
    while(it!=end())
    {
        if(less(*it, x))
            ++it;
        else if(less(x, *it))
            it=end();
        else
            break;
    }
    return it;
};

template<class X>
inline typename SemSolver::PointsSet<2, X>::Iterator
        SemSolver::PointsSet<2, X>::find(const KeyType &x)
{
    Iterator it = begin();
    while(it!=end())
    {
        if(less(*it, x))
            ++it;
        else if(less(x, *it))
            it=end();
        else
            break;
    }
    return it;
};

//! \brief Insert a point if it doesn't exists otherwise do nothing
//! \param x the point to be inserted
//! \return the iterator to the point inserted
template<class X>
inline typename SemSolver::PointsSet<2, X>::Iterator
        SemSolver::PointsSet<2, X>::insert(const ValueType &x)
{
    return insert(begin(), x);
};

//! \brief Insert a point if it doesn't exists otherwise do nothing
//! \param x the point to be inserted
//! \param it iterator to the position guess where to insert new entry
//! \return the iterator to the point inserted
template<class X>
typename SemSolver::PointsSet<2, X>::Iterator
        SemSolver::PointsSet<2, X>::insert(Iterator it, const ValueType &x)
{
    while(it!=begin())
    {
        if(less(x, *it))
            --it;
        else
            break;
    }
    while(it!=end())
    {
        if(less(*it, x) )
            ++it;
        else
            break;
    }
    if(it==end() || less(x, *it))
        return points.insert(it, x);
    return end();
};

//! \brief Insert multiple pairs if they don't exist
//! \param first iterator to the first pair
//! \param last iterator to the last pair
template<class X>
template<class InputIterator>
void SemSolver::PointsSet<2, X>::insert(InputIterator first, InputIterator last)
{
    Iterator it = begin();
    while(first!=last)
        it = insert(it, *first++);
    insert(it, last);
};

//! \brief Get the set size
//! \return The set size
template<class X>
inline typename SemSolver::PointsSet<2, X>::SizeType SemSolver::PointsSet<2, X>::size() const
{
    return points.size();
};

//! \brief Access element
/*! If x matches the key of an element in the container, the function returns a reference
    to its mapped value.
    If x does not match the key of any element in the container, the function inserts a
    new element with that key and returns a reference to its mapped value.
*/
//! \param x Key value of the element whose mapped value is accessed.
//! \return A reference to the element with a key value equal to x.
template<class X>
inline typename SemSolver::PointsSet<2, X>::MappedType &
        SemSolver::PointsSet<2, X>::operator[](const KeyType &x)
{
    return insert(ValueType(x, MappedType()))->second;
};

#endif // POINTSSET_HPP
