#ifndef SEMGEOMETRY_HPP
#define SEMGEOMETRY_HPP

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for describing the geometry of a SemProblem
    /*! Consist of the description of the whole domain, of a set of its
    subdomains and of its boundary edges */
    //! \param d Dimension of the space
    /*! \param X Must be a type for which operations +, -, * and / are defined
             with semantics (approximately) corresponding to those of a
             field in a mathematical sense. Note that, strictly speaking,
             the built-in type int does not fullfil the requirements on a
             field type, since ints correspond to elements of a ring rather
             than a field, especially operation / is not the inverse of * */
    template<int d, class X>
    class SemGeometry
    {
    };
};

#include <SemSolver/pslg.hpp>
#include <SemSolver/polygonation.hpp>

namespace SemSolver
{

    template<class X>
    class SemGeometry<2,X>
    {

        PSLG<X> pslg;
        Polygonation<2,X> sub_domains;

    public:

//! Access geometry description
        inline PSLG<X> const &domain() const
			{
    return pslg;
};

//! Access geometry partition
        inline Polygonation<2,X> const &subDomains() const
{
    return sub_domains;
};

//! Set geometry description
        inline void setDomain(PSLG<X> const &domain)
		{
    pslg = domain;
};

//! Set geometry partition
        inline void setSubDomains(Polygonation<2,X> const &sub_domains)
			{
    this->sub_domains = sub_domains;
};

//! Check if a point doesn't lie outside the domain
        inline bool contains(const Point<2, X> &point) const
{
    return !sub_domains.elementIndicesAt(point).empty();
};
    };
};

#endif // SEMGEOMETRY_HPP
