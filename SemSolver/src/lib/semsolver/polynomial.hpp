#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <vector>

#include <SemSolver/function.hpp>
#include <SemSolver/matrix.hpp>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handing the mathematical concept of polynomials over X
    template<class X>
    class Polynomial
    {
    private:
        // members
        std::vector<X> _coefficients;

    public:
        //! \brief Default constructor
        Polynomial()
            : _coefficients(1,0.)
        {
        }

        //! \brief Construct a constant polynomial
        //! \param scalar The scalar value of the polynomial
        Polynomial(X const &scalar)
            : _coefficients(1,scalar)
        {
        }

        //! \brief Constuct a polynomial from its coefficients
        //! \param coefficients Vector of coefficients from that of order 0
        Polynomial(std::vector<X> const &coefficients)
            : _coefficients(coefficients)
        {
        }

        //! \brief Copy constructor
        //! \param poly the polynomial to be copied
        Polynomial(Polynomial const &poly)
            : _coefficients(poly._coefficients)
        {
        }

        //! \brief Destructor
        ~Polynomial()
        {};

        //! \brief Assignment operator
        //! \param poly the polynomial to be copied
        Polynomial & operator = (Polynomial const &poly)
                                {
            _coefficients = poly._coefficients;
            return *this;
        }

        //! \brief Addition assignment operator
        //! \param poly the polynomial to be summed
        Polynomial & operator += (Polynomial const &poly)
                                 {
            int deg = poly.degree();
            if(deg>degree())
                _coefficients.resize(deg+1,0.);
            for(int i=0; i<=deg; ++i)
                _coefficients[i] += poly._coefficients[i];
            return *this;
        }

        //! \brief Subtraction assignment operator
        //! \param poly the polynomial to be subtracted
        Polynomial & operator -= (Polynomial const &poly)
                                 {
            int deg = poly.degree();
            if(deg>degree())
                _coefficients.resize(deg+1,0.);
            for(int i=0; i<=deg; ++i)
                _coefficients[i] -= poly._coefficients[i];
            return *this;
        }

        //! \brief Multiplication assignment operator
        //! \param poly the polynomial to be multiplied
        Polynomial & operator *= (Polynomial const &poly)
                                 {
            int deg = degree();
            int deg_ = poly.degree();
            std::vector<double> product;
            product.resize(deg+deg_+1,0.);
            for(int i=0; i<=deg; ++i)
                for(int j=0; j<=deg_; ++j)
                    product[i+j] += _coefficients[i]*poly._coefficients[j];
            _coefficients = product;
            return *this;
        }

        //! \brief Division assignment operator
        //! \param poly the polynomial by which divide
        Polynomial & operator /= (Polynomial const &poly)
                                 {
            int deg = degree();
            int deg_ = poly.degree();
            Polynomial quotient, residual;
            residual = *this;
            {
                for(int i=deg; i>=deg_; --i)
                {
                    if (i>=0 && residual._coefficients[i])
                    {
                        Polynomial temp;
                        temp._coefficients.resize(deg-deg_+1,double(0));
                        temp._coefficients[i-deg_] =
                                residual._coefficients[i]/poly._coefficients[deg_];
                        quotient+=temp;
                        residual-=poly*temp;
                    }
                }
            }
            _coefficients = quotient._coefficients;
            return *this;
        }

        //! \brief Modulo assignment operator.
        //! \param poly the divisor used for computing the reminder
        Polynomial & operator %= (const Polynomial &poly)
                                 {
            int deg = degree();
            int deg_ = poly.degree();
            Polynomial quotient, residual;
            residual = *this;
            {
                for(int i=deg; i>=deg_; --i)
                {
                    if (residual._coefficients[i])
                    {
                        Polynomial temp;
                        temp._coefficients.resize(deg-deg_+1,0.);
                        temp._coefficients[i-deg_] =
                                residual._coefficients[i]/poly._coefficients[deg_];
                        quotient+=temp;
                        residual-=poly*temp;
                    }
                }
            }
            _coefficients = residual._coefficients;
            return *this;
        }

        //! \brief Unary plus
        Polynomial operator + () const
        {
            return *this;
        }

        //! \brief Unary minus
        Polynomial operator - () const
        {
            Polynomial result(*this);
            for(std::vector<double>::iterator it=result._coefficients.begin();
            it!=result._coefficients.end(); ++it)
                *it = -*it;
            return result;
        }

        //! \brief Addition operator
        //! \param poly the polynomial to be summed
        Polynomial operator + (const Polynomial &poly) const
        {
            Polynomial sum = *this;
            sum += poly;
            return sum;
        }

        //! \brief Subtraction operator
        //! \param poly the polynomial to be subtracted
        Polynomial operator - (const Polynomial &poly) const
        {
            Polynomial difference = *this;
            difference -= poly;
            return difference;
        }

        //! \brief Multiplication operator
        //! \param poly the polynomial to be multiplied
        Polynomial operator * (const Polynomial &poly) const
        {
            Polynomial product = *this;
            product *= poly;
            return product;
        }

        //! \brief Division operator
        //! \param poly the polynomial by which divide
        Polynomial operator / (const Polynomial &poly) const
        {
            Polynomial quotient = *this;
            quotient *= poly;
            return quotient;
        }

        //! \brief Modulo  operator.
        //! \param poly the divisor used for computing the reminder
        Polynomial operator % (const Polynomial &poly) const
        {
            Polynomial residual = *this;
            residual %= poly;
            return residual;
        }

        //! Equal to operator
        bool operator == (const Polynomial &poly) const
        {
            if(degree()!=poly.degree())
                return false;
            else
                for(int i=0; i<degree(); ++i)
                    if(_coefficients[i]!=poly._coefficients[i])
                        return 0;
            return 1;
        }

        //! Not equal to operator
        bool operator != (const Polynomial &poly) const;

        double operator () (const X &variable) const
        {
            double value=0;
            for(int i=degree(); i>0; --i)
                value = (value + _coefficients[i]) * variable;
            return value+_coefficients[0];
        }

        //! Access coefficient
        //! \param order The order of the coefficient to access
        inline double coefficient(const int &order) const
        {
#ifdef SEMDEBUG
            if(order<0 || order>degree())
                qFatal("SemSolver::Polynomial::coefficient - ERROR : order must be a non"\
                       " negative value not greater than degree");
#endif
            return _coefficients[order];
        }

        //! \brief Get the degree of a polynomial
        int degree() const
        {
            int deg = _coefficients.size()-1;
            while(deg>=0 && !_coefficients[deg])
                --deg;
            return deg;
        }

        //! \brief Set the coefficient of given order
        //! \param order Must be non negative and not greater than degree
        //! \param coefficient The value to be assigned
        inline void setCoefficient(const int &order, X const &coefficient)
        {
#ifdef SEMDEBUG
            if(order<0 || (long)order>=(long)_coefficients.size() )
                qFatal("SemSolver::Polynomial::setCoefficient - ERROR : order "\
                       "out of range.");
#endif
            _coefficients[order]=coefficient;
        };

        //! \brief Set the polynomial degree
        //! \param degree Must be not less than -1
        inline void setDegree(const int &degree)
        {
#ifdef SEMDEBUG
            if(degree<-1)
                qFatal("SemSolver::Polynomial::setDegree - ERROR : degree must be not le"\
                       "ss than -1.");
#endif
            _coefficients.resize(degree+1,0.);
        };

        //! Compute polynomial derivative
        Polynomial derivative() const
        {
            int deg = degree();
            if(deg < 1)
                return Polynomial(0.);
            std::vector<double> derivative;
            derivative.resize(deg,0.);
            for(int i=0; i<deg; ++i)
                derivative[i] = (i+1)*_coefficients[i+1];
            return Polynomial(derivative);
        }

        //! Compute quotient of the division by a monic binomial with Ruffini's rule
        //! zero the 0-order coefficient of the divisor
        Polynomial ruffini(X const &zero) const
        {
            std::vector<double> result;
            int deg=degree();
            result.resize(deg+1,double(0.));
            for(int i=deg; i>0; --i)
                result[i-1] = _coefficients[i] + zero * result[i];
            return Polynomial(result);
        }

        //! Compute the vector of polynomial roots
        std::vector<X> zeros() const
        {
            int deg = degree();
            Matrix<X> companion(deg,deg,0.);
            for(int i=0; i<deg-1; ++i)
            {
                companion[i+1][i] = 1;
                companion[i][deg-1] = -_coefficients[i]/_coefficients[deg];
            }
            Vector<X> eigenvalues(companion.realEigenvalues());
            std::vector<double> zeros;
            for(int i=0; i< deg; ++i)
                zeros.push_back(eigenvalues[i]);
            return zeros;
        }
    };
}

#endif // POLYNOMIAL_HPP
