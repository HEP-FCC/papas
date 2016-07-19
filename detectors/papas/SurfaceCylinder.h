//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef SURFACECYLINDER_H
#define SURFACECYLINDER_H

#include "Definitions.h"

namespace papas {
/**
  Component which is used to define a detector element
 */
class SurfaceCylinder {
public:
  SurfaceCylinder(papas::Position clayer, double rad = 0., double z = 0.);
  double z() const { return m_z; };
  double getRadius() const { return m_radius; }
  papas::Position layer() const { return m_layer; }

private:
  papas::Position m_layer;
  double m_radius;
  double m_z;
};

}  // end namespace papas
#endif
