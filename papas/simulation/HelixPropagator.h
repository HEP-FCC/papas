
#ifndef helixpropagator_h
#define helixpropagator_h
#include "papas/detectors/Detector.h"

namespace papas {
class PFParticle;
class HelixPropagator {
/** Class to determine where the (helix) path of a charged particle crosses the detector cyclinders
*/
public:
  /** Constructor
   field Magnetic field in Tesla(?) (assumed constant)
  */
  HelixPropagator(double field);
  /**
   Propagate charged particle along Helix to the selected cylinder and store the point
   where the particle crosses the cylinder
   ptc particle that is to be propagated
   cyl cylinder to which the particle is to be propagated.
   */
  virtual void propagateOne(const PFParticle& ptc, const SurfaceCylinder& cyl) const;

private:
  double m_field;///<Magnetic field in Tesla (?)
  /**
   Propagate charged particle along Helix to the selected cylinder and store the point
   where the particle crosses the cylinder
   ptc particle that is to be propagated
   layer name of layer.This will be used to label the point in the path points
   cylinderz the z value of the end of the cylinder
   cylinderRadius Radius of the cylinder
   */
  void propagateOne(const PFParticle& ptc, papas::Position layer, double cylinderz, double cylinderRadius,
                    const Field& field, bool debugInfo) const;
};
}  // end namespace papas
#endif
