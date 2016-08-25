
#ifndef propogator_h
#define propogator_h
#include "Detector.h"

namespace papas {
class SimParticle;

/// Determines where an uncharged particle path crosses a detector cylinder
class Propagator {
public:
  Propagator(){};
  // virtual void propagateOne(SimParticle& ptc, Id::Layer Layer, bool inner = true) = 0;
  virtual void propagateOne(SimParticle& ptc, const SurfaceCylinder& cyl) = 0;

protected:
  virtual void propagateOne(SimParticle& ptc, papas::Position layer, double cylinderz, double cylinderRadius) = 0;
};

}  // end namespace papas
#endif