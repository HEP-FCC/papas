#include "papas/simulation/HelixPropagator.h"

#include "papas/datatypes/Helix.h"
#include "papas/datatypes/Particle.h"
#include "papas/detectors/Field.h"
#include "papas/utility/GeoTools.h"

namespace papas {

HelixPropagator::HelixPropagator(std::shared_ptr<const Field> field) : Propagator(field) {}

void HelixPropagator::setPath(Particle& ptc) const {
  if (ptc.path() == nullptr) {
    auto helix = std::make_shared<Helix>(Helix(ptc.p4(), ptc.startVertex(), ptc.charge(), m_field->getMagnitude()));
    ptc.setPath(helix);
  }
}

void HelixPropagator::propagateOne(const Particle& ptc, const SurfaceCylinder& cyl) const {
  /*if (ptc.path() == nullptr) {
    auto helix = std::make_shared<Helix>(Helix(ptc.p4(), ptc.startVertex(), ptc.charge(), m_field->getMagnitude()));
    ptc.setPath(helix);
  }*/
  auto helix = std::static_pointer_cast<Helix>(ptc.path());

  bool is_looper = helix->extremePointXY().Mag() < cyl.radius();
  double udir_z = helix->unitDirection().Z();

  if (!is_looper) {
    try {
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

    catch (std::string s) {
      return;
    }
  }
  if (is_looper) {
    double destz = cyl.z();
    if (udir_z < 0.) destz = -destz;
    TVector3 destination = helix->pointAtZ(destz);
    helix->addPoint(cyl.layer(), destination);
  }
}

}  // end namespace papas
