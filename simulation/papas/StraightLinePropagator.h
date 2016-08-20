
#ifndef straightlinepropagator_h
#define straightlinepropagator_h
#include "Detector.h"

namespace papas {
class SimParticle;

class StraightLinePropagator {
public:
  StraightLinePropagator();
  void propagateOne(SimParticle& ptc, const SurfaceCylinder& cyl);
private:
  void propagateOne(SimParticle& ptc, papas::Position layer, double cylinderz, double cylinderRadius);
};

}  // end namespace papas
#endif