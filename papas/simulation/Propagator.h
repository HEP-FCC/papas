#ifndef propogator_h
#define propogator_h


namespace papas {
class PFParticle;
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
  virtual void propagateOne(const PFParticle& ptc, const SurfaceCylinder& cyl, double field = 0) const = 0;
  /**
   Propagate particle all cylinders of the detector
   @param[in] ptc particle that is to be propagated
   @param[in] detector  Detector through which to propagate
   */
  void propagate(const PFParticle& ptc, const Detector& detector) const;
};

}  // end namespace papas
#endif
