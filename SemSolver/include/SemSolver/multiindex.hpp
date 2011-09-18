#ifndef MULTIINDEX_HPP
#define MULTIINDEX_HPP

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class multi-index notation
    //! \param N Length of the multi-index
    template<int N>
    class MultiIndex
    {
        int _indices[N];

    public:
        //! \brief Construct zero multi-index
        MultiIndex()
        {
            for(int i=0; i<N; ++i)
                _indices[i] = 0;
        };

        //! \brief Construct multi-index
        //! \param sub_indices Index components
        MultiIndex(int const sub_indices[N])
        {
            for(int i=0; i<N; ++i)
                _indices[i] = sub_indices[i];
        };

        //! \brief Copy constructor
        //! \param index The MultiIndex to be copied
        MultiIndex(MultiIndex<N> const &index)
        {
            for(int i=0; i<N; ++i)
                _indices[i] = index.subIndex(i);
        };

        //! \brief Get a component
        //! \param index The component index
        //! \return The component value
        int const &subIndex(int const &index) const
        {
            if(index<0 || N<=index)
                qFatal("SemSolver::MultiIndex::subIndex - ERROR : index out of range.");
            return _indices[index];
        };

        //! \brief Set a component
        //! \param index The component index
        //! \param sub_index The component value
        void setSubIndex(int const &index, int const &sub_index)
        {
            if(index<0 || N<=index)
                qFatal("SemSolver::MultiIndex::subIndex - ERROR : index out of range.");
            _indices[index] = sub_index;
        };

        //! \brief Partial order
        struct less
        {
            bool operator() (MultiIndex<N> const &mi0,
                             MultiIndex<N> const &mi1) const
            {
                for(int i=0; i<N; ++i)
                {
                    if(mi0.subIndex(i)<mi1.subIndex(i))
                        return true;
                    if(mi0.subIndex(i)>mi1.subIndex(i))
                        return false;
                }
                return false;
            };
        };
    };
};

#endif // MULTIINDEX_HPP
