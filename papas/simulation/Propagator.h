#ifndef propagator_h
#define propagator_h

#include <memory>

namespace papas {
class Field;
class Particle;
class Detector;
class SurfaceCylinder;

class Propagator {
  /** Virtual class to be used to determine where a particle path crosses a detector cylinder
  */
public:
  /** Constructor
   */
  Propagator(std::shared_ptr<const Field> field) : m_field(field){};

  /**
   Propagate particle to the selected cylinder and store the point where the particle crossed the cylinder
   @param[in] ptc particle that is to be propagated
   @param[in] cyl cylinder to which the particle is to be propagated.
   @param[in] field magnitude of magnetic field (used only for charged particles, defaults to zero if not set)
   */
  virtual void propagateOne(Particle& ptc, const SurfaceCylinder& cyl) const = 0;

  /**  Propagate particle all cylinders of the detector
    @param[in] ptc particle that is to be propagated
    @param[in] detector  Detector through which to propagate
    */
  void propagate(Particle& ptc, const Detector& detector) const;

protected:
  std::shared_ptr<const Field> m_field;
};

}  // end namespace papas
#endif
