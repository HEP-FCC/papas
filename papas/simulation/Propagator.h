#ifndef propogator_h
#define propogator_h


namespace papas {

class Particle;
class Detector;
class PFParticle;
class SurfaceCylinder;

class Propagator {
/** Virtual class to be used to determine where a particle path crosses a detector cylinder
*/
public:
  /** Constructor
   */
  Propagator(){};
  /**
   Propagate particle to the selected cylinder and store the point where the particle crossed the cylinder
   @param[in] ptc particle that is to be propagated
   @param[in] cyl cylinder to which the particle is to be propagated.
   @param[in] field magnitude of magnetic field (used only for charged particles, defaults to zero if not set)
   */
  virtual void propagateOne(Particle& ptc, const SurfaceCylinder& cyl, double field = 0) const = 0;
 /**
   Propagate particle all cylinders of the detector
   @param[in] ptc particle that is to be propagated
   @param[in] detector  Detector through which to propagate
   */
  void propagate(Particle& ptc, const Detector& detector);
};

}  // end namespace papas
#endif
