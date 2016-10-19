//
//  Created by Alice Robson on 09/11/15.
//
//
#include "papas/detectors/SurfaceCylinder.h"
#include "TVector3.h"
#include <cmath>
#include <iostream>

namespace papas {

SurfaceCylinder::SurfaceCylinder(papas::Position layer, double rad, double z) : m_layer(layer), m_radius(rad), m_z(z) {
  if (m_radius < 0) throw "Invalid cyclinder radius";
}

}  // end namespace papas
