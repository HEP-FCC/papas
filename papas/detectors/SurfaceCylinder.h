//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef SURFACECYLINDER_H
#define SURFACECYLINDER_H

#include "papas/datatypes/Definitions.h"

namespace papas {
/**
  Component which is used to define a detector element such as  a calorimeter
 */
class SurfaceCylinder {
public:
  /** Constructor
   * @param[in] papas::Position clayer : eg kEcalIn, kEcalDecay etc
   * @param[in] double rad : cyclinder radius
   * @param[in] double z : cyclinder maximum z value
   */
  SurfaceCylinder(papas::Position clayer, double rad = 0., double z = 0.);
  double z() const { return m_z; };                  ///<z value
  double radius() const { return m_radius; }         ///<cyclinder radius
  papas::Position layer() const { return m_layer; }  ///<cyclinder layer

private:
  papas::Position m_layer;
  double m_radius;
  double m_z;
};

}  // end namespace papas
#endif
