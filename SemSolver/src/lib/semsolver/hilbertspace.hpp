#ifndef HILBERTSPACE_HPP
#define HILBERTSPACE_HPP

#include <cmath>

#include <vector>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief prototype class for handling the concept of Hilbert Space
    //! \param Function the type of the elements of the space
    //! \param X The type returned by scalar product
    template<class Function, class X>
    class HilbertSpace
    {
        std::vector<Function> _base;

    public:
        //! \brief Class for handling space elements as Fourier coefficients
        class Element
        : std::vector<X>
        {
            HilbertSpace<Function, X> const *_space;

        public:
            //! \brief Constructor
            Element(const HilbertSpace<Function, X> *space) : _space(space) { };

            //! \brief Destructor
            virtual ~Element() {};
        };

        //! \brief Get a base function
        //! \param index of the base function to be accessed
        //! \return Pointer to function
        virtual Function const *baseFunction(int const &index) const
        {
#if SEMDEBUG
            if(index<0 || index>=dimension())
                qFatal("SemSolver::HilbertSpace::baseFunction - ERROR : index out of ran"\
                       "ge.");
#endif
            return &_base[index];
        };

        //! \brief Evaluate the scalar product between two elements
        //! \return Default constructed X
        virtual X scalarProduct(Element const &,
                                Element const &) const
        {
            return X();
        };

        //! \brief Evaluate the norm of an element
        //! \param element
        //! \return Square root of scalar product of element with itself
        virtual inline X norm(Element element)
        {
            return std::sqrt(scalarProduct(element, element));
        };

        //! \brief Get the dimension of the space
        //! \return Base size
        virtual inline int dimension() const
        {
            return _base.size();
        };

        //! \brief Get the projection of a generic function on the Hilbert Space
        //! \return Space element
        virtual Element projection(Function const *) const
        {
            return Element(this);
        };
    };
};

#endif // HILBERTSPACE_HPP
