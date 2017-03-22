
#ifndef helixpropagator_h
#define helixpropagator_h
#include "papas/detectors/Detector.h"
#include "papas/simulation/Propagator.h"

namespace papas {
class Particle;
  
class HelixPropagator : public Propagator {
/** Class to determine where the (helix) path of a charged particle crosses the detector cyclinders
*/
public:
  /** Constructor
  */
  HelixPropagator();
  /**
   Propagate charged particle along Helix to the selected cylinder and store the point
   where the particle crosses the cylinder
   ptc particle that is to be propagated
   cyl cylinder to which the particle is to be propagated.
   */
  virtual void propagateOne(const Particle& ptc, const SurfaceCylinder& cyl, double field) const override;


};
}  // end namespace papas
#endif
