#ifndef straightlinepropagator_h
#define straightlinepropagator_h

#include "papas/simulation/Propagator.h"

#include <memory>

namespace papas {
class Particle;
class Field;
class SurfaceCylinder;

class StraightLinePropagator : public Propagator {
  /// Calculates where an uncharged particle crosses a detector cyclinder
public:
  /** Constructor
  */
  StraightLinePropagator(std::shared_ptr<const Field> field);
  /**
   Propagate uncharged particle along a strightline to the selected cylinder and
   store the point where the particle crossed the cylinder
   @param[in] ptc particle that is to be propagated
   @param[in] cyl cylinder to which the particle is to be propagated.
   @param[in] field magnitude of magnetic field (not used for uncharged particles)
  */
  void propagateOne(const Particle& ptc, const SurfaceCylinder& cyl) const override;

  /** Sets the particle path to a straightline
   @param[in] ptc particle that is to be propagated
   */
  void setPath(Particle& ptc) const override;

private:
};

}  // end namespace papas
#endif
