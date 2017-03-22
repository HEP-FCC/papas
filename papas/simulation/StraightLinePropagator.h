
#ifndef straightlinepropagator_h
#define straightlinepropagator_h
#include "papas/detectors/Detector.h"
#include "papas/simulation/Propagator.h"

namespace papas {
class PFParticle;

class StraightLinePropagator  : public Propagator {
/// Calculates where an uncharged particle crosses a detector cyclinder
public:
  /** Constructor
  */
  StraightLinePropagator();
  /**
   Propagate uncharged particle along a strightline to the selected cylinder and
   store the point where the particle crossed the cylinder
   ptc particle that is to be propagated
   cyl cylinder to which the particle is to be propagated.
  */
  void propagateOne(const PFParticle& ptc, const SurfaceCylinder& cyl, double field = 0) const override;
private:
};

}  // end namespace papas
#endif
