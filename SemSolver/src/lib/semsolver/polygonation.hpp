#ifndef POLYGONATION_HPP
#define POLYGONATION_HPP

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handling polygonations
    //! \param d Dimension of the space
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<int d, class X>
    class Polygonation;
};

#include <vector>

#include <SemSolver/polygon.hpp>
#include <SemSolver/point.hpp>

//! \brief Project main namespace
namespace SemSolver
{

    //! \brief Class for handling 2D polygonations
    /*! \param X Must be a type for which operations +, -, * and / are defined
                 with semantics (approximately) corresponding to those of a
                 field in a mathematical sense. Note that, strictly speaking,
                 the built-in type int does not fullfil the requirements on a
                 field type, since ints correspond to elements of a ring rather
                 than a field, especially operation / is not the inverse of * */
    template<class X>
    class Polygonation<2,X>
    {

    public:

        //! \brief A Polygonation element
        /*! It can be a triangle, a quadrangle, or any Polygon. It also stores information
            about its neighbours */
        class Element
        {

        private:
            Polygon<2, X>    polygon;
            std::vector<int> neighbours_vector;

        public:
            //! \brief Default constructor
            inline Element();

            //! \brief Construct an Element from a given Polygon and Neighbours list
            //! \param geometry The geometry of the element
            //! \param neighbours Vector of neighbour element ids in counterclokwise order
            inline Element(Polygon<2, X> const &geometry,
                           std::vector<int> const &neighbours);

            //! \brief Get the element geometry
            //! return The Element's Polygon
            inline Polygon<2, X> const &geometry() const;

            //! \brief Get the position of a vertex
            //! \param vertex The Point to search
            /*! \return The position (starting at 0) of the vertex if there is,
                        -1 otherwise */
            unsigned vertexPosition(Point<2, X> const &vertex) const;

            //! \brief Clear Element's content
            inline void clear();

            //! \brief Get a vertex of an Element
            //! \param index The vertex position
            //! \return The vertex
            inline Point<2, X> vertex(int const &index) const;

            //! \brief Get an Elemnt size
            //! \return The number of vertices
            inline int size() const;

            //! \brief Get iterator to first vertex
            //! \return Constant iterator
            inline typename Polygon<2, X>::Vertex_const_iterator verticesBegin() const;

            //! \brief Get iterator to last vertex
            //! \return Constant iterator
            inline typename Polygon<2, X>::Vertex_const_iterator verticesEnd() const;

            //! \brief Get iterator to first neghbour id
            //! \return Constant iterator
            inline std::vector<int>::const_iterator neighboursBegin() const;

            //! \brief Get iterator to last neghbour id
            //! \return Constant iterator
            inline std::vector<int>::const_iterator neighboursEnd() const;

            //! \brief Set Element's geometry from a Point's sequence
            //! \param first Pointer to the first Point of the sequence
            //! \param last  Pointer to the last Point of the sequence
            inline void setGeometry(Point<2,X> const *first,
                                    Point<2,X> const *last);

            //! \brief Set a neighbour id
            //! \param index Posiition of the neighbour
            //! \param id    Id of the neighbour
            inline void setNeighbour(const unsigned &index,
                                     int const &id);

            //! \brief Get a neighbour id
            //! \param index Posiition of the neighbour
            //! \return Id of the neighbour
            inline int neighbour(const unsigned &index) const;

            //! \brief Test if a point lies on the Element
            //! \param  point The Point to test with
            //! \return The test result
            inline bool contains(Point<2, X> const &point) const;

        };

    private:
        std::vector<Element> elements;

    public:
        //! \brief Test if Polygonation is a quadrangualtion
        //! \return The test result
        bool isQuadrangulation() const;

        //! \brief Refine the Polygonation
        /*! Split all elements into subelements, one for each vertex, by adding as
            vertices the centroid of each element and the midpoint of each segment */
        void refine();

        //! \brief Clear the Polygonation
        inline void clear();

        //! Get an Element
        //! \param index Element's position
        //! \return reference to Element
        inline const Element &element(const unsigned &index) const;

        //! Get Polygonation size
        //! \return The number of elements
        inline unsigned size() const;

        //! Add an Element to the Polygonation
        //! \param polygon The geometry of the element
        //! \param neighbours Vector of neighbour element ids in counterclokwise order
        inline void addElement(Polygon<2, X> const &polygon,
                               std::vector<int> const &neighbours);

        //! Get on which Elements lies a Point
        //! \param point The Point to check
        //! \return Vector of position of that elements that contain the given point
        std::vector<unsigned> elementIndicesAt(Point<2, X> const &point) const;
    };
};

template<class X>
bool SemSolver::Polygonation<2, X>::isQuadrangulation() const
{
    for(typename std::vector<Element>::const_iterator it = elements.begin();
    it != elements.end(); ++it)
    {
        if(it->size()!=4)
            return false;
    }
    return true;
};

template<class X>
void SemSolver::Polygonation<2, X>::refine()
{
    int *subelements_first = new int[size()];
    subelements_first[0]=0;
    for(unsigned i=0; i<size()-1; ++i)
        subelements_first[i+1]=subelements_first[i]+element(i).size();
    std::vector<Element> subelements;
    for(unsigned i=0; i<size(); ++i)
    {
        Element const &old_element = element(i);
        Point<2,X> cent = centroid(old_element.verticesBegin(),
                                   old_element.verticesEnd());
        Point<2,X> vertices[4];
        Polygonation::Element subelement;
        int n = old_element.size();
        for(int j=0; j<n; ++j)
        {
            vertices[0] = old_element.vertex(j);
            vertices[1] = midpoint(old_element.vertex(j),
                                   old_element.vertex((j+1)%n));
            vertices[2] = cent;
            vertices[3] = midpoint(old_element.vertex(j),
                                   old_element.vertex((j+n-1)%n));
            subelement.setGeometry(vertices, vertices+4);
            if(old_element.neighbour(j)<0) // border
                subelement.setNeighbour(0,old_element.neighbour(j));
            else
            {
                int neigh = old_element.neighbour(j)-1;
                int neigh_first = subelements_first[neigh];
                element(neigh);
                old_element.vertex(j);
                int pos = element(neigh).vertexPosition(old_element.vertex(j));
                subelement.setNeighbour(0,neigh_first+pos+1);
            }
            if(old_element.neighbour((j+1)%n)<0)
                subelement.setNeighbour(1, old_element.neighbour((j+1)%n));
            else
            {
                int neigh = old_element.neighbour((j+1)%n)-1;
                int neigh_first = subelements_first[neigh];
                int pos = element(neigh).vertexPosition(old_element.vertex(j));
                subelement.setNeighbour(1, neigh_first+pos+1);
            }
            subelement.setNeighbour(2,subelements_first[i]+(j+1)%n+1);
            subelement.setNeighbour(3,subelements_first[i]+(j+n-1)%n+1);
            subelements.push_back(subelement);
        }
    }
    elements.clear();
    elements = subelements;
};

template<class X>
inline void SemSolver::Polygonation<2, X>::clear()
{
    elements.clear();
};

template<class X>
inline const typename SemSolver::Polygonation<2, X>::Element
        &SemSolver::Polygonation<2, X>::element(const unsigned &index) const
{
#ifdef SEMDEBUG
    if(index>=elements.size())
        qFatal("SemSolver::Polygonation::element - ERROR : index out of bounds.");
#endif //SEMDEBUG
    return elements[index];
};

template<class X>
inline unsigned SemSolver::Polygonation<2, X>::size() const
{
    return elements.size();
};

template<class X>
inline void SemSolver::Polygonation<2, X>::addElement(Polygon<2, X> const &polygon,
                                                      std::vector<int> const &neighbours)
{
    elements.push_back(Element(polygon, neighbours));
};

template<class X>
std::vector<unsigned> SemSolver::Polygonation<2, X>::elementIndicesAt(
        SemSolver::Point<2, X> const &point) const
{
    std::vector<unsigned> indices;
    for(unsigned i=0; i<size(); ++i)
        if(element(i).contains(point))
            indices.push_back(i);
    return indices;
};

template<class X>
inline SemSolver::Polygonation<2, X>::Element::Element()
{};

template<class X>
inline SemSolver::Polygonation<2, X>::Element::Element(Polygon<2, X> const &geometry,
                                                       std::vector<int> const &neighbours)
{
    polygon = geometry;
    neighbours_vector = neighbours;
};

template<class X>
inline SemSolver::Polygon<2, X> const &SemSolver::Polygonation<2, X>::Element::geometry()
        const
{
    return polygon;
}

template<class X>
unsigned SemSolver::Polygonation<2, X>::Element::vertexPosition(Point<2, X> const &vertex)
        const
{
    for(int i=0; i<polygon.size(); ++i)
    {
        if(polygon.vertex(i) == vertex)
        {
            return i;
        }
    }
    return -1;
};

template<class X>
inline void SemSolver::Polygonation<2, X>::Element::clear()
{
    polygon.clear();
    neighbours_vector.clear();
};

template<class X>
inline SemSolver::Point<2, X> SemSolver::Polygonation<2, X>::Element::vertex(
        int const &index) const
{
    return polygon.vertex(index);
};

template<class X>
inline int SemSolver::Polygonation<2, X>::Element::size() const
{
    return polygon.size();
};

template<class X>
inline typename SemSolver::Polygon<2, X>::Vertex_const_iterator
        SemSolver::Polygonation<2, X>::Element::verticesBegin() const
{
    return polygon.vertices_begin();
};

template<class X>
inline typename SemSolver::Polygon<2, X>::Vertex_const_iterator
        SemSolver::Polygonation<2, X>::Element::verticesEnd() const
{
    return polygon.vertices_end();
};

template<class X>
inline std::vector<int>::const_iterator
        SemSolver::Polygonation<2, X>::Element::neighboursBegin() const
{
    return neighbours_vector.begin();
};

template<class X>
inline std::vector<int>::const_iterator
        SemSolver::Polygonation<2, X>::Element::neighboursEnd() const
{
    return neighbours_vector.end();
};

template<class X>
inline void SemSolver::Polygonation<2, X>::Element::setGeometry(Point<2,X> const *first,
                                                                Point<2,X> const *last)
{
    polygon = Polygon<2,X>(first, last);
    neighbours_vector.resize(polygon.size(),0);
};

template<class X>
inline void SemSolver::Polygonation<2, X>::Element::setNeighbour(const unsigned &index,
                                                                 int const &id)
{
#ifdef SEMDEBUG
    if(index>=neighbours_vector.size())
        qFatal("SemSolver::Polygonation::Element::setNeighbour - ERROR : index out of bo"\
               "unds.");
#endif //SEMDEBUG
    neighbours_vector[index] = id;
};

template<class X>
inline int SemSolver::Polygonation<2, X>::Element::neighbour(const unsigned &index)
        const
{
#ifdef SEMDEBUG
    if(index>=neighbours_vector.size())
        qFatal("SemSolver::Polygonation::Element::neighbour - ERROR : index out of bound"\
               "s.");
#endif //SEMDEBUG
    return neighbours_vector[index];
};

template<class X>
inline bool SemSolver::Polygonation<2, X>::Element::contains(Point<2, X> const &point)
        const
{
    return polygon.contains(point);
};

#endif // POLYGONATION_HPP
