
#ifndef propogator_h
#define propogator_h
#include "papas/detectors/Detector.h"
#include "papas/detectors/SurfaceCylinder.h"
class SurfaceCylinder;

namespace papas {
class PFParticle;

class Propagator {
/** Virtual class to be used to determine where a particle path crosses a detector cylinder
*/
public:
  /** Constructor
   */
  Propagator(){};
  /**
   Propagate particle to the selected cylinder and store the point where the particle crossed the cylinder
   ptc particle that is to be propagated
   cyl cylinder to which the particle is to be propagated.
   */
  virtual void propagateOne(const PFParticle& ptc, const SurfaceCylinder& cyl, double field = 0) = 0;
  void propagate(const PFParticle& ptc, std::list<std::shared_ptr< const SurfaceCylinder>> cylinders, double field = 0 );
};

}  // end namespace papas
#endif
