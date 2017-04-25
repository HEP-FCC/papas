#include "papas/simulation/Propagator.h"

#include "papas/detectors/Detector.h"
#include "papas/detectors/Field.h"

namespace papas {
void Propagator::propagate(const Particle& ptc, const Detector& detector) const {
  for (const auto el : detector.elements())
    propagateOne(ptc, el->volumeCylinder().inner());
}
}  // end namespace papas
