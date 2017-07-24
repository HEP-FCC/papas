#ifndef ClicFIELD_H
#define ClicFIELD_H

#include "papas/detectors/Field.h"

namespace papas {

// Forward declaration
class VolumeCylinder;
/// Clic specific implementation of Detector Field element
///
class ClicField : public Field {
public:
  /** Constructor
   * @param[in] magnitude field strength
   * @param[in] field cylinder radius
   * @param[in] field cylinder z
   * @param[in] field cylinder material x0
   * @param[in] field cylinder material lambdaI
   */
  ClicField(double magnitude = 2., double radius = 3.5, double z = 4.8, double x0 = 0., double lambdaI = 0.);

private:
};

}  // end namespace papas
#endif
