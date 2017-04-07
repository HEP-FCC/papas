#include "papas/detectors/VolumeCylinder.h"

#include <cmath>

#include "TVector3.h"

namespace papas {

VolumeCylinder::VolumeCylinder(papas::Layer layer, double outerrad, double outerz, double innerrad, double innerz)
    : m_outer(papas::Position::kHcalOut, outerrad, outerz), m_inner(papas::Position::kHcalIn, innerrad, innerz) {

  if (innerrad > outerrad) {
    throw "ERROR: outer radius of subtracted cylinder must be smaller";
  } else if (innerz > outerz) {
    throw "ERROR: outer z of subtracted cylinder must be smaller";
  }

  if (layer == papas::Layer::kEcal) {
    m_outer = SurfaceCylinder(papas::Position::kEcalOut, outerrad, outerz);
    m_inner = SurfaceCylinder(papas::Position::kEcalIn, innerrad, innerz);
  }
}

VolumeCylinder::~VolumeCylinder() {}

bool VolumeCylinder::contains(const TVector3& point) const {
  double_t perp = point.Perp();
  if (std::abs(point.Z()) < m_inner.z()) {
    return (perp >= m_inner.radius()) & (perp < m_outer.radius());
  } else if (std::abs(point.Z()) < m_outer.z()) {
    return perp < m_outer.radius();
  } else
    return false;
}

}  // end namespace papas
