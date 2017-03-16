
#ifndef propogator_h
#define propogator_h
#include "papas/detectors/Detector.h"

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
  virtual void propagateOne(Particle& ptc, const SurfaceCylinder& cyl) = 0;

protected:
  /**
   Propagate particle to the selected cylinder and store the point where the particle crossed the cylinder
   ptc particle that is to be propagated
   layer name of layer.This will be used to label the point in the path points
   cylinderz the z value of the end of the cylinder
   cylinderRadius Radius of the cylinder
   */
  virtual void propagateOne(Particle& ptc, papas::Position layer, double cylinderz, double cylinderRadius) = 0;
};

}  // end namespace papas
#endif
