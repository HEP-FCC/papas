#include "papas/simulation/Propagator.h"
#include "papas/detectors/Detector.h"
#include "papas/detectors/VolumeCylinder.h"
#include "papas/datatypes/PFParticle.h"


namespace papas {
  void Propagator::propagate(const PFParticle& ptc, const Detector& detector) {
    for (const auto el: detector.elements())
      propagateOne(ptc, el->volumeCylinder().inner(), detector.field()->getMagnitude());
}
}//end namespace papas