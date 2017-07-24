#include "papas/detectors/SurfaceCylinder.h"

#include "TVector3.h"

#include <cmath>
#include <iostream>

#include "papas/utility/StringFormatter.h"

namespace papas {

SurfaceCylinder::SurfaceCylinder(papas::Position layer, double rad, double z) : m_layer(layer), m_radius(rad), m_z(z) {
  if (m_radius < 0) throw "Invalid cyclinder radius";
}

std::string SurfaceCylinder::info() const { return string_format("Cylinder : R=%5.2f, z=%5.2f", m_radius, m_z); }

std::ostream& operator<<(std::ostream& os, const SurfaceCylinder& cylinder) {
  os << cylinder.info();
  return os;
}

double SurfaceCylinder::thetaJunction() const {
  // Return theta of the barrel endcap junction
  return atan(m_radius / m_z);
}

double SurfaceCylinder::etaJunction() const {
  // Return eta of the barrel endcap junction'''
  return -log(tan(thetaJunction() / 2.));
}
}  // end namespace papas
