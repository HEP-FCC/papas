#ifndef SURFACECYLINDER_H
#define SURFACECYLINDER_H

#include "papas/datatypes/Definitions.h"

#include <ostream>

namespace papas {
/**
  Component which is used to define a detector element such as  a calorimeter
 */
class SurfaceCylinder {
public:
  /** Constructor
   * @param[in] papas::Position clayer : eg kEcalIn, kEcalDecay etc
   * @param[in] double rad : cylinder radius
   * @param[in] double z : cylinder maximum z value
   */
  SurfaceCylinder(papas::Position clayer, double rad = 0., double z = 0.);
  double z() const { return m_z; };                  ///<z value
  double radius() const { return m_radius; }         ///<cylinder radius
  double thetaJunction() const;                      ///<theta of barrel endcap junction
  double etaJunction() const;                        ///<eta of barrel endcap junction
  papas::Position layer() const { return m_layer; }  ///<cylinder layer
  std::string info() const;

private:
  papas::Position m_layer;  ///<cylinder layer
  double m_radius;          ///<cylinder radius
  double m_z;               ///<z value
};
std::ostream& operator<<(std::ostream& os, const SurfaceCylinder& cylinder);

}  // end namespace papas
#endif
