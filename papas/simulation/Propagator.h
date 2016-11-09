
#ifndef propogator_h
#define propogator_h
#include "papas/detectors/Detector.h"

namespace papas {
class SimParticle;

/// Determines where an uncharged particle path crosses a detector cylinder
class Propagator {
public:
  Propagator(){};
  virtual void propagateOne(const SimParticle& ptc, const SurfaceCylinder& cyl) = 0;

protected:
  virtual void propagateOne(const SimParticle& ptc, papas::Position layer, double cylinderz, double cylinderRadius) = 0;
};

}  // end namespace papas
#endif
