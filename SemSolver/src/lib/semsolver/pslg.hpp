#ifndef PSLG_HPP
#define PSLG_HPP

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handing Planar Straight Line Graphs
    //! \param X The type of coordinates
    template<class X>
    class PSLG;
};

#include <cmath>

//! \brief Project main namespace
namespace SemSolver
{
    //! \brief Class for handing Planar Straight Line Graphs
    /*! By definition, a PSLG is just a list of vertices and segments. It can also
        contain information about holes and concavities, as well as regional attributes
        and constraints on the areas of triangles. */
    //! \param X The type of coordinates
    template<class X>
    class PSLG
    {
    public:

        //! \brief PSLG Vertex struct
        /*! Contain coordinates x and y, vertex number, attributes - which are typically
            values of physical quantities (such as mass or conductivity) associated
            with the nodes of a finite element mesh and boundary markers. Boundary
            markers are used to identify boundary vertices and vertices resting on
            PSLG segments. */
        struct Vertex
        {
            int number;
            X x;
            X y;
            double *attributes;
            int marker;
        };

        //! \brief PSLG Segment struct
        /*! Segment is specified by listing the indices of its two endpoints.
            This means that you must include its endpoints in the vertex list.
            Each segment, like each vertex, may have a boundary marker. */
        struct Segment
        {
            int number;
            int source;
            int target;
            int marker;
        };

        //! \brief PSLG Hole struct
        /*! Holes are specified by identifying a point inside each hole. */
        struct Hole
        {
            int number;
            X x;
            X y;
        };

    private:

        unsigned   vertices_number;
        unsigned   dimension;
        unsigned   vertices_attributes_number;
        unsigned   vertices_boundary_markers_number;
        Vertex *vertices_list;

        unsigned    segments_number;
        unsigned    segments_boundary_markers_number;
        Segment *segments_list;

        unsigned holes_number;
        Hole *holes_list;

    public:

        //! Default constructor
        PSLG();

        //! Destructor
        ~PSLG();

        //! Clear PSLG content
        void clear();

        //! Get a vertex
        //! \param index The vertex position
        //! \return The vertex
        inline Vertex const &vertex(const unsigned &index) const;

        //! Get a segment
        //! \param index The segment position
        //! \return The segment
        inline Segment const &segment(const unsigned &index) const;

        //! Get a hole
        //! \param index The hole position
        //! \return The hole
        inline Hole const &hole(const unsigned &index) const;

        //! Get number of vertices
        //! \return Vertices number
        inline unsigned const &vertices() const;

        //! Get number of segments
        //! \return Segments number
        inline unsigned const &segments() const;

        //! Get number of holes
        //! \return Holes number
        inline unsigned const &holes() const;

        //! Set number of vertices
        //! \param number Vertices number
        void setNumberOfVertices(const unsigned &number);

        //! Set number of segments
        //! \param number Segments number
        void setNumberOfSegments(const unsigned &number);

        //! Set number of holes
        //! \param number Holes number
        void setNumberOfHoles(const unsigned &number);

        //! Set number of vertices attributes
        //! \param number Vertices attributes number
        void setNumberOfVerticesAttributes(const unsigned &number);

        //! Set number of vertices boundary markers
        //! \param number Vertices boundary markers number
        inline void setNumberOfVerticesBoundaryMarkers(const unsigned &number);

        //! Set a vertex
        //! \param index The vertex position
        //! \param number The vertex number
        //! \param x Abscissa
        //! \param y Ordinate
        //! \param attributes Attributes array
        //! \param marker Boundary marker
        void setVertex(unsigned const &index,
                       int const &number,
                       const X &x,
                       const X &y,
                       double *attributes = 0,
                       int const &marker = 0);

        //! Set a segment
        //! \param index The segment position
        //! \param number The segment number
        //! \param source First end-point number
        //! \param target Second end-point number
        //! \param marker Boundary marker
        void setSegment(unsigned const &index,
                        int const &number,
                        int const &source,
                        int const &target,
                        int const &marker = 0);

        //! Set a hole
        //! \param index The hole position
        //! \param number The hole number
        //! \param x Abscissa
        //! \param y Ordinate
        void setHole(unsigned const &index,
                     int const &number,
                     const X &x,
                     const X &y);
    };
};

template<class X>
SemSolver::PSLG<X>::PSLG()
{
    vertices_number = 0;
    dimension = 2;
    vertices_attributes_number = 0;
    vertices_boundary_markers_number = 0;
    vertices_list = 0;
    segments_number = 0;
    segments_boundary_markers_number = 0;
    segments_list = 0;
    holes_number = 0;
    holes_list = 0;
};

template<class X>
SemSolver::PSLG<X>::~PSLG()
{
    for(unsigned i=0; i<vertices_number; ++i)
    {
        delete [] vertices_list[i].attributes;
        vertices_list[i].attributes = 0;
    }
    //delete [] vertices_list;
    //delete [] segments_list;
    //delete [] holes_list;
};

template<class X>
void SemSolver::PSLG<X>::clear()
{
    for(unsigned i=0; i<vertices_number; ++i)
    {
        delete [] vertices_list[i].attributes;
        vertices_list[i].attributes = 0;
    }
    delete [] vertices_list;
    delete [] segments_list;
    delete [] holes_list;
    vertices_number = 0;
    vertices_attributes_number = 0;
    vertices_boundary_markers_number = 0;
    vertices_list = 0;
    segments_number = 0;
    segments_boundary_markers_number = 0;
    segments_list = 0;
    holes_number = 0;
    holes_list = 0;
};

template<class X>
inline const typename SemSolver::PSLG<X>::Vertex
        &SemSolver::PSLG<X>::vertex(const unsigned &index) const
{
#ifdef SEMDEBUG
    if(index>=vertices_number)
        qFatal("SemSolver::PSLG::vertex - ERROR : index out of bounds.");
#endif //SEMDEBUG
    return vertices_list[index];
};

template<class X>
inline const typename SemSolver::PSLG<X>::Segment
        &SemSolver::PSLG<X>::segment(const unsigned &index) const
{
#ifdef SEMDEBUG
    if(index>=segments_number)
        qFatal("SemSolver::PSLG::segment - ERROR : index out of bounds.");
#endif //SEMDEBUG
    return segments_list[index];
};

template<class X>
inline const typename SemSolver::PSLG<X>::Hole
        &SemSolver::PSLG<X>::hole(const unsigned &index) const
{
#ifdef SEMDEBUG
    if(index>=holes_number)
        qFatal("SemSolver::PSLG::hole - ERROR : index out of bounds.");
#endif //SEMDEBUG
    return holes_list[index];
};

template<class X>
inline const unsigned &SemSolver::PSLG<X>::vertices() const
{
    return vertices_number;
};

template<class X>
inline const unsigned &SemSolver::PSLG<X>::segments() const
{
    return segments_number;
};

template<class X>
inline const unsigned &SemSolver::PSLG<X>::holes() const
{
    return holes_number;
};

template<class X>
void SemSolver::PSLG<X>::setNumberOfVertices(const unsigned &number)
{
    for(unsigned i=0; i<vertices_number; ++i)
    {
        delete [] vertices_list[i].attributes;
        vertices_list[i].attributes = 0;
    }
    delete [] vertices_list;
    vertices_number = number;
    if(number)
    {
        vertices_list = new Vertex[number];
        if(vertices_attributes_number)
            for(unsigned i=0; i<number; ++i)
                vertices_list[i].attributes = new double[vertices_attributes_number];
        else
            for(unsigned i=0; i<number; ++i)
                vertices_list[i].attributes = 0;
    }
    else
        vertices_list = 0;
};

template<class X>
void SemSolver::PSLG<X>::setNumberOfSegments(const unsigned &number)
{
    delete [] segments_list;
    segments_number = number;
    if(number)
        segments_list = new Segment[number];
    else
        segments_list = 0;
};

template<class X>
void SemSolver::PSLG<X>::setNumberOfHoles(const unsigned &number)
{
    delete [] holes_list;
    holes_number = number;
    if(number)
        holes_list = new Hole[number];
    else
        holes_list = 0;
};

template<class X>
void SemSolver::PSLG<X>::setNumberOfVerticesAttributes(const unsigned &number)
{
    vertices_attributes_number = number;
    if(number)
    {
        for(unsigned i=0; i<vertices_number; ++i)
        {
            delete [] vertices_list[i].attributes;
            vertices_list[i].attributes = new double[number];
        }
    }
    else
    {
        for(unsigned i=0; i<vertices_number; ++i)
        {
            delete [] vertices_list[i].attributes;
            vertices_list[i].attributes = 0;
        }
    }
};

template<class X>
void SemSolver::PSLG<X>::setNumberOfVerticesBoundaryMarkers(const unsigned &number)
{
#ifdef SEMDEBUG
    if(number!=0 && number !=1)
        qFatal("SemSolver::PSLG::setNumberOfVerticesBoundaryMarkers - ERROR : number mus"\
               "t be 0 or 1.");
#endif //SEMDEBUG
    vertices_boundary_markers_number = number;
};

template<class X>
void SemSolver::PSLG<X>::setVertex(const unsigned &index,
                                   int const &number,
                                   const X &x,
                                   const X &y,
                                   double *attributes,
                                   int const &marker)
{
#ifdef SEMDEBUG
    if(index>=vertices_number)
        qFatal("SemSolver::PSLG::setVertex - ERROR : index out of bounds.");
#endif //SEMDEBUG
    vertices_list[index].number = number;
    vertices_list[index].x = x;
    vertices_list[index].y = y;
    delete [] vertices_list[index].attributes;
    if(vertices_attributes_number)
    {
        vertices_list[index].attributes = new double[vertices_attributes_number];
        for (unsigned i=0; i<vertices_attributes_number; ++i)
            vertices_list[index].attributes[i] = attributes[i];
    }
    else
        vertices_list[index].attributes = 0;
    vertices_list[index].marker = marker;
};

template<class X>
void SemSolver::PSLG<X>::setSegment(const unsigned &index,
                                    int const &number,
                                    int const &source,
                                    int const &target,
                                    int const &marker)
{
#ifdef SEMDEBUG
    if(index>=segments_number)
        qFatal("SemSolver::PSLG::setSegment- ERROR : index out of bounds.");
#endif //SEMDEBUG
    segments_list[index].number = number;
    segments_list[index].source = source;
    segments_list[index].target = target;
    segments_list[index].marker = marker;
};

template<class X>
void SemSolver::PSLG<X>::setHole(const unsigned &index,
                                 int const &number,
                                 const X &x,
                                 const X &y)
{
#ifdef SEMDEBUG
    if(index>=holes_number)
        qFatal("SemSolver::PSLG::setHole - ERROR : index out of bounds.");
#endif //SEMDEBUG
    holes_list[index].number = number;
    holes_list[index].x = x;
    holes_list[index].y = y;
};

#endif // PSLG_HPP

