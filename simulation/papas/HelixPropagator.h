
#ifndef helixpropagator_h
#define helixpropagator_h
#include "Detector.h"

namespace papas {
class SimParticle;
/// Class to determine where the (helix) path of a charged particle crosses the detector cyclinders
class HelixPropagator {
public:
  HelixPropagator(double field);
  virtual void propagateOne(const SimParticle& ptc, const SurfaceCylinder& cyl) const;

private:
  double m_field;
  void propagateOne(const SimParticle& ptc, papas::Position layer, double cylinderz, double cylinderRadius,
                    const Field& field, bool debugInfo) const;
};
}  // end namespace papas
#endif
