//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  GEOMETRY_H
#define GEOMETRY_H

#include <memory>
#include <string>
#include "TVector3.h"
#include "Definitions.h"


namespace papas {
/**
  Component of Volume which is used to define a detector element
 */
class SurfaceCylinder {
public:
  
   SurfaceCylinder(papas::Position clayer, double rad = 0., double z = 0.);
   double Z() const {return m_z;};
   double getRadius() const {return m_radius;}
  //std::string getName() const  {return m_name;}
   papas::Position Layer() const {return m_layer;}
private:
  //const std::string m_name;
   papas::Position m_layer;
   double m_radius;
   double m_z;
};

/**
  Defines inner and outer cyclinders of a detector element
 */
class VolumeCylinder {
public:
  VolumeCylinder(papas::Layer layer, double outerrad, double outerz, double innerrad = 0., double innerz = 0.);
   bool Contains(const TVector3& point) const;
   const SurfaceCylinder& inner() const {return m_inner;}
   const SurfaceCylinder& outer() const {return m_outer;}
   papas::Position innerLayer() const {return m_inner.Layer();}
  ~VolumeCylinder();
private:
  //const std::string m_name;
   papas::Layer m_layer;
   SurfaceCylinder m_outer;
   SurfaceCylinder m_inner;
};
} // end namespace papas
#endif
