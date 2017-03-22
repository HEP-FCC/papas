#include "papas/simulation/Propagator.h"


namespace papas {
  void Propagator::propagate(const PFParticle& ptc, std::list<std::shared_ptr<const SurfaceCylinder> >cylinders, double field) {
  for (const auto cyl: cylinders)
    propagateOne(ptc, *cyl, field);
}
}