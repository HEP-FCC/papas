

//#include <iostream>
#include "papas/simulation/HelixPropagator.h"
#include "papas/utility/GeoTools.h"
#include "papas/datatypes/Helix.h"
#include "papas/datatypes/Path.h"
#include "papas/datatypes/SimParticle.h"

namespace papas {

HelixPropagator::HelixPropagator(double field) : m_field(field) {}

void HelixPropagator::propagateOne(const SimParticle& ptc, const SurfaceCylinder& cyl) const {
  auto helix = std::static_pointer_cast<Helix>(ptc.path());

  bool is_looper = helix->extremePointXY().Mag() < cyl.radius();
  double udir_z = helix->unitDirection().Z();

  if (!is_looper) {
    auto intersect = circleIntersection(helix->centerXY().X(), helix->centerXY().Y(), helix->rho(), cyl.radius());

    double phi_m = helix->phi(intersect[0].first, intersect[0].second);
    double phi_p = helix->phi(intersect[1].first, intersect[1].second);

    TVector3 destination = helix->pointAtPhi(phi_p);
    if (destination.Z() * udir_z < 0.) {
      destination = helix->pointAtPhi(phi_m);
    }

    if (fabs(destination.Z()) < cyl.z()) {
      helix->addPoint(cyl.layer(), destination);
    } else
      is_looper = true;
  }
  if (is_looper) {
    double destz = cyl.z();
    if (udir_z < 0.) destz = -destz;
    TVector3 destination = helix->pointAtZ(destz);
    helix->addPoint(cyl.layer(), destination);
  }
}

}  // end namespace papas
