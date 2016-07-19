//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef VOLUMECYLINDER_H
#define VOLUMECYLINDER_H

#include "Definitions.h"
#include "SurfaceCylinder.h"
#include "TVector3.h"
#include <memory>
#include <string>

namespace papas {

/**
  Defines inner and outer cyclinders of a detector element
 */
class VolumeCylinder {
public:
  VolumeCylinder(papas::Layer layer, double outerrad, double outerz, double innerrad = 0., double innerz = 0.);
  bool contains(const TVector3& point) const;
  const SurfaceCylinder& inner() const { return m_inner; }
  const SurfaceCylinder& outer() const { return m_outer; }
  papas::Position innerLayer() const { return m_inner.layer(); }
  ~VolumeCylinder();

private:
  SurfaceCylinder m_outer;
  SurfaceCylinder m_inner;
};
}  // end namespace papas
#endif
