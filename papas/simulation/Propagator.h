
#ifndef propogator_h
#define propogator_h
#include "papas/detectors/Detector.h"
#include "papas/detectors/SurfaceCylinder.h"
class SurfaceCylinder;

namespace papas {
class Particle;

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
  virtual void propagateOne(const Particle& ptc, const SurfaceCylinder& cyl, double field = 0) const = 0;
  void propagate(const Particle& ptc, const Detector& detector);

};

}  // end namespace papas
#endif
