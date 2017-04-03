//
//  Created by Alice Robson on 09/11/15.
//
//
#include "papas/detectors/SurfaceCylinder.h"
#include "TVector3.h"
#include "papas/utility/PDebug.h"
#include <cmath>

namespace papas {

SurfaceCylinder::SurfaceCylinder(papas::Position layer, double rad, double z) : m_layer(layer), m_radius(rad), m_z(z) {
  if (m_radius < 0) throw "Invalid cyclinder radius";
}

std::string SurfaceCylinder::info() const { return string_format("Cylinder : R=%5.2f, z=%5.2f", m_radius, m_z); }

std::ostream& operator<<(std::ostream& os, const SurfaceCylinder& cylinder) {
  os << cylinder.info();
  return os;
}

}  // end namespace papas