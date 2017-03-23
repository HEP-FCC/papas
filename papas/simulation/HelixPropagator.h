
#ifndef helixpropagator_h
#define helixpropagator_h
#include "papas/detectors/Detector.h"
#include "papas/simulation/Propagator.h"

namespace papas {
class PFParticle;
  
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
   @param[in] ptc particle that is to be propagated
   @param[in] cyl cylinder to which the particle is to be propagated.
   @param[in] field magnitude of magnetic field
   */
  virtual void propagateOne(const PFParticle& ptc, const SurfaceCylinder& cyl, double field) const override;

};
}  // end namespace papas
#endif
