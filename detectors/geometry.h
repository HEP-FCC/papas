//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  GEOMETRY_H
#define GEOMETRY_H

#include <memory>
#include <string>
#include "TVector3.h"

//SurfaceCylinder
/**
  Component of Volume which is used to define a detector element
 */
class SurfaceCylinder {
public:
   SurfaceCylinder(const std::string& name, double rad = 0., double z = 0.);

   double Z() const {return m_z;};
   double getRadius() const {return m_radius;};
   std::string getName() const  {return m_name;} ;

private:
   const std::string m_name;
   double m_radius;
   double m_z;
};

//VolumeCylinder
/**
  Defines inner and outer cyclinders of a detector element
 */
class VolumeCylinder {
public:
   VolumeCylinder(const std::string& name, double outerrad, double outerz,
                  double innerrad = 0., double innerz = 0.);
   bool Contains(const TVector3& point) const;
   const SurfaceCylinder& Inner() const {return m_inner;}; 
   const SurfaceCylinder& Outer() const {return m_outer;};
   const std::string InnerName() const {return m_inner.getName();};
   ~VolumeCylinder();

private:
   const std::string m_name;
   SurfaceCylinder m_outer;
   SurfaceCylinder m_inner;

};

#endif