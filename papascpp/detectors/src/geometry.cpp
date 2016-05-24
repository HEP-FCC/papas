//
//  Created by Alice Robson on 09/11/15.
//
//
#include <iostream>
#include <cmath>
#include "TVector3.h"
#include "geometry.h"

namespace papas {

SurfaceCylinder::SurfaceCylinder(papas::Position layer, double rad ,double z):
   m_layer(layer), m_radius(rad), m_z(z)
{
  }


VolumeCylinder::VolumeCylinder(papas::Layer layer, double outerrad, double outerz, double innerrad, double innerz):
  //m_layer(layer),
m_outer(papas::Position::kHcalOut, outerrad, outerz),
m_inner(papas::Position::kHcalIn, innerrad, innerz)
{
 
  
  if (layer==papas::Layer::kEcal) {
    m_outer = SurfaceCylinder(papas::Position::kEcalOut, outerrad, outerz);
    m_inner = SurfaceCylinder(papas::Position::kEcalIn, innerrad, innerz);
  }
 
  
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

} // end namespace papas

