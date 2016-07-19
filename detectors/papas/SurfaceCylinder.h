//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef SURFACECYLINDER_H
#define SURFACECYLINDER_H

#include "Definitions.h"
#include "TVector3.h"
#include <memory>
#include <string>

namespace papas {
/**
  Component of Volume which is used to define a detector element
 */
class SurfaceCylinder {
public:
  SurfaceCylinder(papas::Position clayer, double rad = 0., double z = 0.);
  double Z() const { return m_z; };
  double getRadius() const { return m_radius; }
  // std::string getName() const  {return m_name;}
  papas::Position Layer() const { return m_layer; }

private:
  // const std::string m_name;
  papas::Position m_layer;
  double m_radius;
  double m_z;
};

}  // end namespace papas
#endif
