#ifndef SEMSPACE_HPP
#define SEMSPACE_HPP

namespace SemSolver
{
    template <int d, class X>
            class SemSpace;
}

#include <vector>
#include <cmath>

#include <SemSolver/hilbertspace.hpp>
#include <SemSolver/semfunction.hpp>
#include <SemSolver/multiindex.hpp>
#include <SemSolver/semgeometry.hpp>
#include <SemSolver/semparameters.hpp>
#include <SemSolver/pointsmap.hpp>

namespace SemSolver
{
    /*! Class used for modeling the mathematical concept of Functional Space of
    Spectral Element Functions */
    //! \param d Dimension of the domain
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template <int d, class X>
            class SemSpace
                : public HilbertSpace< SemFunction<d,X>, X >
    {};

    /*! Class used for modeling the mathematical concept of Functional Space of
    Spectral Element Functions on 2D Euclidean space */
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<class X>
    class SemSpace<2,X>
        : public HilbertSpace< SemFunction<2,X>, X >
    {
    public:
        //! Class for space nodes
        //! It stores information about the subdomains and borders of which is member
        class Node
        {
            typedef std::vector< MultiIndex<2> > Indices2Vector;
            typedef std::vector< MultiIndex<3> > Indices3Vector;

            Point<2,X> _point;
            Indices3Vector _element_indices;
            Indices2Vector _border_indices;

            //! Specify that node is on a subdomain
            //! \param index = (i0, i1, i2). Node is on subdomain i0 as (i1, i2)-th node
            void addSubDomainIndex(MultiIndex<3> const &index)
            {
                MultiIndex<3>::less compare;
                Indices3Vector::iterator it;
                for(it=_element_indices.begin();
                it!=_element_indices.end();
                ++it)
                {
                    if(compare(*it,index))
                        continue;
                    else if(compare(index,*it))
                        break;
                }
                _element_indices.insert(it,index);
            };

            //! Specify that node is on a border
            //! \param index = (i0, i1). Node is on border i0 as i1-th node
            void addBorderIndex(MultiIndex<2> const &index)
            {
                MultiIndex<2>::less compare;
                Indices2Vector::iterator it;
                for(it=_border_indices.begin();
                it!=_border_indices.end();
                ++it)
                {
                    if(compare(*it,index))
                        continue;
                    else if(compare(index,*it))
                        break;
                }
                _border_indices.insert(it,index);
            };

                public:
            //! Construct node from a point
            Node( Point<2,X> const &point)
                : _point(point) {};

            //! Access node point
            Point<2,X> const &point() const
            {
                return _point;
            };

            //! Get number of subdomain of which node is member
            inline int supportSubDomains() const
            {
                return _element_indices.size();
            };

            //! Get the index-th subdomain index of wich node is member
            MultiIndex<3> const &subDomainIndex(int const &index) const
            {
                if(index<0 || supportSubDomains()<=index)
                    qFatal("SemSolver::SemSpace::Node::subDomainIndex - ERROR : there is"\
                           "no element with index element_index.");
                return _element_indices[index];
            };

            //! Get number of borders of which node is member
            inline int supportBorders() const
            {
                return _border_indices.size();
            };

            //! Get the index-th border index of wich node is member
            MultiIndex<2> const &borderIndex(int const &index) const
            {
                if(index<0 || supportBorders()<=index)
                    qFatal("");
                return _border_indices[index];
            };
            friend class SemSpace;
        };

        //! Class for handling members of the space as Fourier coefficients
        class Element
            : public Function<Point<2,X>, X>
        {
            SemSpace const *_space;
            std::vector<X> _coefficients;
            std::vector< SemFunction<2,X> *> _base;

        public:
            //! Construct space element from Fourier coefficients
            Element(SemSpace const *space, Vector<X> const &coefficients)
                : _space(space)
            {
                for (int i=0; i<coefficients.dim(); ++i)
                {
                    _coefficients.push_back(coefficients[i]);
                }
            };
            ~Element() {};

            //! Compute element value at a point
            X evaluate(Point<2,X> const &x) const
            {
                double result=0;
                if(_space->_geometry.contains(x))
                {
                    for(unsigned i=0; i<_space->nodes(); ++i)
                    {
                        result += _coefficients[i]*_space->baseFunction(i)->evaluate(x);
                    }
                }
                return result;
            };
        };

        typedef MultiIndex<2>::less Index2Order;
        typedef MultiIndex<3>::less Index3Order;
        typedef std::vector<Node> NodesVector;
        typedef std::vector< SemFunction<2,X> * > SemFunctionsVector;
        typedef PointsMap<2, X, int> NodesMap;
        typedef typename NodesMap::ConstIterator NodeConstIterator;
        typedef std::map< MultiIndex<3>, int, Index3Order > ElementsMap;
        typedef typename ElementsMap::const_iterator ElementConstIterator;
        typedef std::map< MultiIndex<2>, MultiIndex<3>, Index2Order >\
                BordersMap;
        typedef std::vector<int> BordersVector;
        typedef std::map< MultiIndex<3>, double, Index3Order > WeightsMap;
        typedef typename Polygonation<2,X>::Element SubDomain;

    protected:

        SemParameters<X> const &_parameters;
        SemGeometry<2,X> const &_geometry;

    private:
        NodesVector _nodes;
        SemFunctionsVector _base;

        NodesMap _point_map;
        ElementsMap _element_map;
        BordersMap _border_map;
        std::map<int, int> _border_ids;
        BordersVector _borders;
        WeightsMap _weights;

        //! Add index-th subdomain node to space
        inline int addSubDomainNode(MultiIndex<3> const &index,
                                    Point<2,X> const &point)
        {
            int i;
            NodeConstIterator it = _point_map.find(point);
            if(it==_point_map.end())
            {
                i = _nodes.size();
                _point_map.insert(point, i);
                _nodes.push_back(point);      
            }
            else
                i = it->second;
            _nodes[i].addSubDomainIndex(index);

            _element_map[index] = i;
            return i;
        };

        inline int addBorderNode(MultiIndex<2> const &border_index,
                                 MultiIndex<3> const &element_index)
        {
            ElementConstIterator it = _element_map.find(element_index);
#ifdef SEMDEBUG
            if(it==_element_map.end())
                qFatal("SemSolver::SemSpace::addBorderNode - ERROR : there is no element"\
                       " with index element_index.");
#endif
            int i = it->second;
            _nodes[i].addBorderIndex(border_index);
            _border_map[border_index] = element_index;
            return i;
        };

        inline void addWeight(MultiIndex<3> const &index, double const &weight)
        {
            _weights[index] = weight;
        };

        inline int setBaseRestrictionPolynomialFunciton(int const &index,
                                                        int const &element_index,
                                                        Polynomial<X> const &px,
                                                        Polynomial<X> const &py)
        {
            _base[index]->setPolynomialComponent(element_index,0,px);
            _base[index]->setPolynomialComponent(element_index,1,py);
            return index;
        };

    public:

        //! Construct SpectralElement Spece on Spectral Element Geometry and Parameters
        SemSpace(SemGeometry<2,X> const &geometry,
                 SemParameters<X> const &parameters)
                     : _parameters(parameters),
                     _geometry(geometry),
                     _point_map(parameters.tolerance())
        {
            int N = parameters.degree();
            int M = subDomains();

            // compute legendre polynomials

            Polynomial<X> _1; // _legendre_0
            Polynomial<X> _x; // _legendre_1
            Polynomial<X> LN;
            Polynomial<X> DLN;
            {
                _1.setDegree(0);
                _1.setCoefficient(0,1.);
                _x.setDegree(1);
                _x.setCoefficient(0,0.);
                _x.setCoefficient(1,1.);

                Polynomial<X> _legendre_k1 = _1;
                Polynomial<X> _legendre_k2 = _x;
                Polynomial<X> t1, t2;
                for (int k=1; k<N; ++k)
                {
                    t1 = _x * _legendre_k2;
                    t1 *= (2.*k+1.)/(k+1.);
                    t2 = _legendre_k1;
                    t2 *= k/(k+1.);
                    _legendre_k1 = _legendre_k2;
                    _legendre_k2 = t1-t2;
                }
                LN = _legendre_k2;
                DLN = LN.derivative();
            }

            // compute GLL nodes

            std::vector<double> gll_nodes = DLN.zeros();
            gll_nodes.push_back(-1.);
            gll_nodes.push_back(1.);
            std::sort(gll_nodes.begin(), gll_nodes.end());

            // compute GLL polynomials

            std::vector< Polynomial<X> > gll_poly(N+1);
            {
                // i = 0
                gll_poly[0] = _1-_x; // (1-x^2) / (x-x_i)
                gll_poly[0] *= DLN; // [ (1-x^2) * DLN(x) ] / (x-x_i)
                gll_poly[0] /= -N*(N+1)*LN(-1.); // (-1) / [ N*(N+1) ] * [ (1-x^2)
                // * DLN(x) ] / [ (x-x_i) * LN(x_i) ]

                // 0 < i < N
                for (int i=1; i<N; ++i)
                {
                    gll_poly[i] = DLN.ruffini(gll_nodes[i]); // DLN(x) / (x-x_i)
                    gll_poly[i] *= _1-_x*_x; // [ (1-x^2) * DLN(x) ] / (x-x_i)
                    gll_poly[i] /= -N*(N+1)*LN(gll_nodes[i]); // (-1) / [ N*(N+1) ]
                    // * [ (1-x^2) * DLN(x) ]
                    // / [ (x-x_i) * LN(x_i) ]
                }

                // i = N
                gll_poly[N] = _1+_x; // (-1) * (1-x^2) / (x-x_i)
                gll_poly[N] *= DLN; // (-1) * [ (1-x^2) * DLN(x) ] / (x-x_i)
                gll_poly[N] /= N*(N+1)*LN(1.); // (-1) / [ N*(N+1) ] * [ (1-x^2)
                // * DLN(x) ] / [ (x-x_i) * LN(x_i) ]
            }

            // compute GLL weights

            std::vector<double> gll_weights(N+1);
            for( int i=0; i<=N; ++i)
            {
                gll_weights[i] =  2./(N*(N+1.));
                gll_weights[i] /= LN(gll_nodes[i]) * LN(gll_nodes[i]);
            }

            // compute maps

            std::vector< BilinearTransformation<X> > maps;
            for(int i=0; i<M; ++i)
            {
                SubDomain const &element = geometry.subDomains().element(i);
                maps.push_back( BilinearTransformation<X>(element.geometry(),
                                                          _parameters.tolerance()) );
            }

            // get borders ids form PSLG
            for(unsigned i=0; i<geometry.domain().segments(); ++i)
                _border_ids[i] = geometry.domain().segment(i).number;

            // compute nodes

            Point<2,X> x_hat;
            Point<2,X> x;
            double weight;
            MultiIndex<3> element_index;
            MultiIndex<2> border_index;

            for(int i=0; i<M; ++i)
            {
                int left = _geometry.subDomains().element(i).neighbour(0);
                int bottom = _geometry.subDomains().element(i).neighbour(1);
                int right = _geometry.subDomains().element(i).neighbour(2);
                int top = _geometry.subDomains().element(i).neighbour(3);
                if(left<0) // left is on boundary
                {
                    _borders.push_back(-left-1);
                    left = borders();
                }
                else
                    left=0;
                if(bottom<0) // bottom is on boundary
                {
                    _borders.push_back(-bottom-1);
                    bottom = borders();
                }
                else
                    bottom=0;
                if(right<0) // right is on boundary
                {
                    _borders.push_back(-right-1);
                    right = borders();
                }
                else
                    right=0;
                if(top<0) // top is on boundary
                {
                    _borders.push_back(-top-1);
                    top = borders();
                }
                else
                    top=0;

                // bottom left vertex
                x_hat = Point<2,X>(gll_nodes[0],gll_nodes[0]);
                x = maps[i].evaluate(x_hat);

                element_index.setSubIndex(0,i);
                element_index.setSubIndex(1,0);
                element_index.setSubIndex(2,0);
                weight = gll_weights[0]*gll_weights[0]*\
                         std::abs(maps[i].evaluateJacobianDeterminant(x_hat));
                addWeight(element_index,weight);
                addSubDomainNode(element_index, x);

                if(bottom)
                {
                    border_index.setSubIndex(0,bottom);
                    border_index.setSubIndex(1,0);
                    addBorderNode(border_index,element_index);
                }
                if(left)
                {
                    border_index.setSubIndex(0,left);
                    border_index.setSubIndex(1,0);
                    addBorderNode(border_index,element_index);
                }

                // bottom edge
                for (int j=1; j<N; ++j)
                {
                    x_hat = Point<2,X>(gll_nodes[j],gll_nodes[0]);
                    x = maps[i].evaluate(x_hat);

                    element_index.setSubIndex(0,i);
                    element_index.setSubIndex(1,j);
                    element_index.setSubIndex(2,0);
                    weight = gll_weights[j]*gll_weights[0]*\
                             std::abs(maps[i].evaluateJacobianDeterminant(x_hat));
                    addWeight(element_index,weight);
                    addSubDomainNode(element_index, x);

                    if(bottom)
                    {
                        border_index.setSubIndex(0,bottom);
                        border_index.setSubIndex(1,j);
                        addBorderNode(border_index,element_index);
                    }
                }

                // bottom right vertex
                x_hat = Point<2,X>(gll_nodes[N],gll_nodes[0]);
                x = maps[i].evaluate(x_hat);

                element_index.setSubIndex(0,i);
                element_index.setSubIndex(1,N);
                element_index.setSubIndex(2,0);
                weight = gll_weights[N]*gll_weights[0]*\
                         std::abs(maps[i].evaluateJacobianDeterminant(x_hat));
                addWeight(element_index,weight);
                addSubDomainNode(element_index, x);

                if(bottom)
                {
                    border_index.setSubIndex(0,bottom);
                    border_index.setSubIndex(1,N);
                    addBorderNode(border_index,element_index);
                }
                if(right)
                {
                    border_index.setSubIndex(0,right);
                    border_index.setSubIndex(1,0);
                    addBorderNode(border_index,element_index);
                }

                for(int k=1; k<N; ++k)
                {
                    // left edge
                    x_hat = Point<2,X>(gll_nodes[0],gll_nodes[k]);
                    x = maps[i].evaluate(x_hat);

                    element_index.setSubIndex(0,i);
                    element_index.setSubIndex(1,0);
                    element_index.setSubIndex(2,k);
                    weight = gll_weights[0]*gll_weights[k]*\
                             std::abs(maps[i].evaluateJacobianDeterminant(x_hat));
                    addWeight(element_index,weight);
                    addSubDomainNode(element_index, x);

                    if(left)
                    {
                        border_index.setSubIndex(0,left);
                        border_index.setSubIndex(1,k);
                        addBorderNode(border_index,element_index);
                    }

                    // interior
                    for(int j=1; j<N; ++j)
                    {
                        x_hat = Point<2,X>(gll_nodes[j],gll_nodes[k]);
                        x = maps[i].evaluate(x_hat);

                        element_index.setSubIndex(0,i);
                        element_index.setSubIndex(1,j);
                        element_index.setSubIndex(2,k);
                        weight = gll_weights[j]*gll_weights[k]*\
                                 std::abs(maps[i].evaluateJacobianDeterminant(x_hat));
                        addWeight(element_index,weight);
                        addSubDomainNode(element_index, x);
                    }

                    // right edge
                    x_hat = Point<2,X>(gll_nodes[N],gll_nodes[k]);
                    x = maps[i].evaluate(x_hat);

                    element_index.setSubIndex(0,i);
                    element_index.setSubIndex(1,N);
                    element_index.setSubIndex(2,k);
                    weight = gll_weights[N]*gll_weights[k]*\
                             std::abs(maps[i].evaluateJacobianDeterminant(x_hat));
                    addWeight(element_index,weight);
                    addSubDomainNode(element_index, x);

                    if(right)
                    {
                        border_index.setSubIndex(0,right);
                        border_index.setSubIndex(1,k);
                        addBorderNode(border_index,element_index);
                    }
                }

                // top left vertex
                x_hat = Point<2,X>(gll_nodes[0],gll_nodes[N]);
                x = maps[i].evaluate(x_hat);

                element_index.setSubIndex(0,i);
                element_index.setSubIndex(1,0);
                element_index.setSubIndex(2,N);
                weight = gll_weights[0]*gll_weights[N]*\
                         std::abs(maps[i].evaluateJacobianDeterminant(x_hat));
                addWeight(element_index,weight);
                addSubDomainNode(element_index, x);

                if(top)
                {
                    border_index.setSubIndex(0,top);
                    border_index.setSubIndex(1,0);
                    addBorderNode(border_index,element_index);
                }
                if(left)
                {
                    border_index.setSubIndex(0,left);
                    border_index.setSubIndex(1,N);
                    addBorderNode(border_index,element_index);
                }

                // top edge
                for(int j=1; j<N; ++j)
                {
                    x_hat = Point<2,X>(gll_nodes[j],gll_nodes[N]);
                    x = maps[i].evaluate(x_hat);

                    element_index.setSubIndex(0,i);
                    element_index.setSubIndex(1,j);
                    element_index.setSubIndex(2,N);
                    weight = gll_weights[j]*gll_weights[N]*\
                             std::abs(maps[i].evaluateJacobianDeterminant(x_hat));
                    addWeight(element_index,weight);
                    addSubDomainNode(element_index, x);

                    if(top)
                    {
                        border_index.setSubIndex(0,top);
                        border_index.setSubIndex(1,j);
                        addBorderNode(border_index,element_index);
                    }
                }

                // top right vertex
                x_hat = Point<2,X>(gll_nodes[N],gll_nodes[N]);
                x = maps[i].evaluate(x_hat);

                element_index.setSubIndex(0,i);
                element_index.setSubIndex(1,N);
                element_index.setSubIndex(2,N);
                weight = gll_weights[N]*gll_weights[N]*\
                         std::abs(maps[i].evaluateJacobianDeterminant(x_hat));
                addWeight(element_index,weight);
                addSubDomainNode(element_index, x);

                if(top)
                {
                    border_index.setSubIndex(0,top);
                    border_index.setSubIndex(1,N);
                    addBorderNode(border_index,element_index);
                }
                if(right)
                {
                    border_index.setSubIndex(0,right);
                    border_index.setSubIndex(1,N);
                    addBorderNode(border_index,element_index);
                }
            }


            // base functions
            std::vector< PolynomialFunction<2,X> > zero_polys(M);
            for(unsigned i=0; i<_nodes.size(); ++i)
                _base.push_back(new SemFunction<2,X>(_geometry,zero_polys,maps));

            for(int i=0; i<M; ++i)
            {
                for(int j=0; j<=N; ++j)
                {
                    for(int k=0; k<=N; ++k)
                    {
                        element_index.setSubIndex(0,i);
                        element_index.setSubIndex(1,j);
                        element_index.setSubIndex(2,k);
                        int index = subDomainIndex(element_index);
                        setBaseRestrictionPolynomialFunciton(index,i,gll_poly[j],
                                                             gll_poly[k]);
                    }
                }
            }
        }

        //! Get number of space nodes
        unsigned nodes() const
        {
            return _nodes.size();
        };

        //! Access index-th node
        inline const Node &node(const unsigned &index) const
        {
            if( index>=nodes() )
                qFatal("SemSolver::SemSpace::node : index out of range");
            return _nodes[index];
        };

        //! Get number of subdomains
        inline int subDomains() const
        {
            return _geometry.subDomains().size();
        };

        //! Get node index correrponding to an element multindex
        inline int const &subDomainIndex(MultiIndex<3> const &index) const
        {
            std::map<MultiIndex<3>, int, MultiIndex<3>::less>::const_iterator it =
                    _element_map.find(index);
#ifdef SEMDEBUG
            if(it==_element_map.end())
                qFatal("SemSolver::SemSpace::subDomainIndex - ERROR : there is no elemen"\
                       "t with multi-index index.");
#endif
            return it->second;
        }

        //! Get node correrponding to an element multindex
        inline Node const &subDomainNode(MultiIndex<3> const &index) const
        {
            return node(subDomainIndex(index));
        };

        //! Get weight index correrponding to an element multindex
        inline double const &subDomainWeight(MultiIndex<3> const &index) const
        {
            std::map<MultiIndex<3>, double, MultiIndex<3>::less>::const_iterator it =
                    _weights.find(index);
            if(it==_weights.end())
            {
                qFatal("SemSolver::SemSpace::subDomainWeight - ERROR : there is no weigh"\
                       "t with multi-index index.");
            }
            return it->second;
        };

        //! Get number of geometry borders
        inline unsigned borders() const
        {
            return _borders.size();
        };

        //! Get index of i-th border
        inline int border(int const &i) const
        {
            return _borders[i];
        };

        //! Get id of border whith index border
        inline int borderId(int const &border) const
        {
            std::map<int,int>::const_iterator it = _border_ids.find(border);
            if(it!=_border_ids.end())
                return it->second;
            return -1;
        };

        //! Get element index corresponding to a border index
        inline MultiIndex<3> const &borderSubDomainIndex(MultiIndex<2> const &index) const
        {
            std::map<MultiIndex<2>, MultiIndex<3>, MultiIndex<2>::less>::const_iterator it
                    = _border_map.find(index);
#ifdef SEMDEBUG
            if(it==_border_map.end())
                qFatal("SemSolver::SemSpace::borderSubDomainIndex - ERROR : there is no "\
                       "border with multi-index index.");
#endif
            return it->second;
        };

        //! Get node index corresponding to a border index
        inline int const &borderIndex(MultiIndex<2> const &index) const
        {
            return subDomainIndex(borderSubDomainIndex(index));
        };

        //! Get node corresponding to a border index
        inline Node const &borderNode(MultiIndex<2> const &index) const
        {
            return subDomainNode(borderSubDomainIndex(index));
        };

        //! Get weight corresponding to a border index
        inline double const &borderWeight(MultiIndex<2> const &index) const
        {
            return subDomainWeight(borderSubDomainIndex(index));
        };

        //! NOT YET IMPLEMENTED
        double scalarProduct(Element first, Element second)
        {
            /********************************************/
            return 0;
        };

        //! Access base function
        SemFunction<2,X> const *baseFunction(const unsigned &index) const
        {
#if SEMDEBUG
            if(index>=nodes())
                qFatal("SemSolver::HilbertSpace::baseFunction - ERROR : index out of ran"\
                       "ge");
#endif
            return _base[index];
        };

        //! Get space degree
        inline int const &degree() const
        {
            return _parameters.degree();
        };
    };
}

#endif // SEMSPACE_HPP
