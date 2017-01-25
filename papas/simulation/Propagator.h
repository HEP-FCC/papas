
#ifndef propogator_h
#define propogator_h
#include "papas/detectors/Detector.h"

namespace papas {
class PFParticle;

/// Determines where an uncharged particle path crosses a detector cylinder
class Propagator {
public:
  Propagator(){};
  virtual void propagateOne(const PFParticle& ptc, const SurfaceCylinder& cyl) = 0;

protected:
  virtual void propagateOne(const PFParticle& ptc, papas::Position layer, double cylinderz, double cylinderRadius) = 0;
};

}  // end namespace papas
#endif
