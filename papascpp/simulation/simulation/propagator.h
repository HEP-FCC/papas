
#ifndef propogator_h
#define propogator_h
#include "detector.h"

namespace papas {
class SimParticle;

class Propagator {
public:
  Propagator();
  // virtual void propagateOne(SimParticle& ptc, Id::Layer Layer, bool inner = true) = 0;
  virtual void propagateOne(SimParticle& ptc, const SurfaceCylinder& cyl) = 0;

protected:
  virtual void propagateOne(SimParticle& ptc, papas::Position layer, double cylinderz, double cylinderradius) = 0;
};

class StraightLinePropagator {
public:
  StraightLinePropagator();
  // void propagateOne(SimParticle& ptc, Id::Layer Layer, bool inner = true) override;
  void propagateOne(SimParticle& ptc, const SurfaceCylinder& cyl);

private:
  void propagateOne(SimParticle& ptc, papas::Position layer, double cylinderz, double cylinderradius);
};

class HelixPropagator {
public:
  HelixPropagator(double field);
  virtual void propagateOne(SimParticle& ptc, const SurfaceCylinder& cyl);

private:
  double m_field;
  void propagateOne(SimParticle& ptc, papas::Position layer, double cylinderz, double cylinderradius,
                    const Field& field, bool debugInfo);
};
}  // end namespace papas
#endif