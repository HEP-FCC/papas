//
//  Created by Alice Robson on 09/11/15.
//
//
#include <iostream>
#include <cmath>
#include "TVector3.h"
#include "geometry.h"

SurfaceCylinder::SurfaceCylinder(const std::string& name, double rad ,
                                 double z): m_name(name), m_radius(rad), m_z(z)
{

}


VolumeCylinder::VolumeCylinder(const std::string& name, double outerrad,
                               double outerz, double innerrad, double innerz) :
   m_name(name),
   m_outer("_" + name + "out", outerrad, outerz),
   m_inner("_" + name + "in", innerrad, innerz)
{
   if (innerrad > outerrad) {
      std::cout << "ERROR: outer radius of subtracted cylinder must be smaller";
   } else if (innerz > outerz) {
      std::cout << "ERROR: outer z of subtracted cylinder must be smaller";
   }
   //AJRTODO define what happens if inner is empty
}

VolumeCylinder::~VolumeCylinder()
{
}


bool VolumeCylinder::Contains(const TVector3& point) const
{
   double_t perp = point.Perp();
   if (std::abs(point.Z()) < m_inner.Z()) {
      return perp >= m_inner.getRadius() & perp < m_outer.getRadius();
   } else if (std::abs(point.Z()) < m_outer.Z()) {
      return perp < m_outer.getRadius();
   } else
      return false;
}