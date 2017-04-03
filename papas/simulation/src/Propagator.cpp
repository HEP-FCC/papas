#include "papas/simulation/Propagator.h"
#include "papas/datatypes/Particle.h"
#include "papas/detectors/Detector.h"
#include "papas/detectors/VolumeCylinder.h"

namespace papas {
void Propagator::propagate(Particle& ptc, const Detector& detector) const {
  for (const auto el : detector.elements())
    propagateOne(ptc, el->volumeCylinder().inner());
}
}  // end namespace papas
