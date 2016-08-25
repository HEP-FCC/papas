
#ifndef helixpropagator_h
#define helixpropagator_h
#include "Detector.h"

namespace papas {
class SimParticle;
/// Class to determine where the (helix) path of a charged particle crosses the detector cyclinders
class HelixPropagator {
public:
  HelixPropagator(double field);
  virtual void propagateOne(SimParticle& ptc, const SurfaceCylinder& cyl);

private:
  double m_field;
  void propagateOne(SimParticle& ptc, papas::Position layer, double cylinderz, double cylinderRadius,
                    const Field& field, bool debugInfo);
};
}  // end namespace papas
#endif