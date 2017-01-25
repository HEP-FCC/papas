
#ifndef straightlinepropagator_h
#define straightlinepropagator_h
#include "papas/detectors/Detector.h"

namespace papas {
class PFParticle;

/// Calculates where an uncharged particle crosses a detector cyclinder
class StraightLinePropagator {
public:
  StraightLinePropagator();
  void propagateOne(const PFParticle& ptc, const SurfaceCylinder& cyl) const;

private:
  void propagateOne(const PFParticle& ptc, papas::Position layer, double cylinderz, double cylinderRadius) const;
};

}  // end namespace papas
#endif
