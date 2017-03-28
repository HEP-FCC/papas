
#ifndef straightlinepropagator_h
#define straightlinepropagator_h
#include "papas/detectors/Detector.h"
#include "papas/simulation/Propagator.h"

namespace papas {
class Particle;

class StraightLinePropagator  : public Propagator {
/// Calculates where an uncharged particle crosses a detector cyclinder
public:
  /** Constructor
  */
  StraightLinePropagator();
  /**
   Propagate uncharged particle along a strightline to the selected cylinder and
   store the point where the particle crossed the cylinder
   @param[in] ptc particle that is to be propagated
   @param[in] cyl cylinder to which the particle is to be propagated.
   @param[in] field magnitude of magnetic field (not used for uncharged particles)
  */

  void propagateOne(Particle& ptc, const SurfaceCylinder& cyl, double field = 0) const override;
private:
};

}  // end namespace papas
#endif
