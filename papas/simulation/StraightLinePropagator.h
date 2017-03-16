
#ifndef straightlinepropagator_h
#define straightlinepropagator_h
#include "papas/detectors/Detector.h"

namespace papas {
class PFParticle;


class StraightLinePropagator {
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
  void propagateOne(Particle& ptc, const SurfaceCylinder& cyl) const;
private:
  /**
   Propagate uncharged particle along a strightline to the selected cylinder and
   store the point where the particle crossed the cylinder
   ptc particle that is to be propagated
   layer name of layer.This will be used to label the point in the path points
   cylinderz the z value of the end of the cylinder
   cylinderRadius Radius of the cylinder
   */
  void propagateOne(Particle& ptc, papas::Position layer, double cylinderz, double cylinderRadius) const;
};

}  // end namespace papas
#endif
