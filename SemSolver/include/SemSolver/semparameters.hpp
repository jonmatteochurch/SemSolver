#ifndef SEMPARAMETERS_HPP
#define SEMPARAMETERS_HPP

namespace SemSolver
{
    template<class X>
    class SemParameters;
};

namespace SemSolver
{
    //! Class used for stroing the parameters of the spectral element method
    //! It consist of polynomial degree to be used, tolerance and penality coefficient
    template <class X>
    class SemParameters
    {
        int _degree;
        X _tolerance;
        X _penality;

    public:
        //! Default constructor
        SemParameters()
        {};

        //! Construct Parameters from degree, tolerance and penality values
        SemParameters(int const &degree,
                      X const &tolerance,
                      X const &penality)
                          : _degree(degree),
                          _tolerance(tolerance),
                          _penality(penality)
        {};

        //! Access degree parameter
        inline int const &degree() const
        {
            return _degree;
        };

        //! Access tolerance parameter
        inline X const &tolerance() const
        {
            return _tolerance;
        };

        //! Access penality parameter
        inline X const &penality() const
        {
            return _penality;
        };

        //! Set degree parameter
        inline void setDegree(const int &d)
        {
            _degree = d;
        };

        //! Set tolerance parameter
        inline void setTolerance(const X &t)
        {
            _tolerance = t;
        };

        //! Set penality parameter
        inline void setPenality(const X &p)
        {
            _penality = p;
        };
    };
};

#endif // SEMPARAMETERS_HPP
