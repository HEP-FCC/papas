#ifndef CMSFIELD_H
#define CMSFIELD_H

#include "papas/detectors/Field.h"

namespace papas {

/// CMS specific implementation of Detector Field element
///
class CMSField : public Field {
public:
  /** Constructor
   *
   * @param[in] magnitude field strength
   * @param[in] radius field cylinder radius
   * @param[in] z field cylinder z
   * @param[in] x0 field cylinder material X0
   * @param[in] lambdaI field cylinder material lambdaI
   */
  CMSField(double magnitude = 3.8, double radius = 2.9, double z = 3.6, double x0 = 0., double lambdaI = 0.);

private:
};

}  // end namespace papas
#endif
