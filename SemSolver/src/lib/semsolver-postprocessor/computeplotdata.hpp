#ifndef COMPUTEPLOTDATA_HPP
#define COMPUTEPLOTDATA_HPP

#include <SemSolver/semspace.hpp>
#include <SemSolver/vector.hpp>

#include <qwt3d_types.h>

namespace SemSolver
{
    namespace PostProcessor
    {
        template<class X>
        void compute_plot_data(const SemSpace<2, X> &space,
                               const Vector<X> &u,
                               Qwt3D::TripleField &data,
                               Qwt3D::CellField &poly)

        {
            for(int i=0; i<u.rows(); ++i)
                data.push_back(Qwt3D::Triple(space.node(i).point().x(), space.node(i).point().y(), u[i]));
            for(int i=0; i<space.subDomains(); ++i)
            {
                for(int j=0; j<space.degree(); ++j)
                {
                    for(int k=0; k<space.degree(); ++k)
                    {
                        Qwt3D::Cell cell;
                        MultiIndex<3> mi;
                        mi.setSubIndex(0,i);
                        mi.setSubIndex(1,j);
                        mi.setSubIndex(2,k);
                        cell.push_back(space.subDomainIndex(mi));
                        mi.setSubIndex(0,i);
                        mi.setSubIndex(1,j+1);
                        mi.setSubIndex(2,k);
                        cell.push_back(space.subDomainIndex(mi));
                        mi.setSubIndex(0,i);
                        mi.setSubIndex(1,j+1);
                        mi.setSubIndex(2,k+1);
                        cell.push_back(space.subDomainIndex(mi));
                        mi.setSubIndex(0,i);
                        mi.setSubIndex(1,j);
                        mi.setSubIndex(2,k+1);
                        cell.push_back(space.subDomainIndex(mi));
                        poly.push_back(cell);
                    }
                }
            }
        }
    }
};

#endif // COMPUTEPLOTDATA_HPP
