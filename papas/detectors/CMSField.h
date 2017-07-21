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
   * @param[in] magnitude of field strength
   * @param[in] radius field cyclinder radius
   * @param[in] z field cyclinder z
   * @param[in] x0 field cyclinder material X0
   * @param[in] lambdaI field cyclinder material lambdaI
   */
  CMSField(double magnitude, double radius, double z, double x0, double lambdaI);

private:
};

}  // end namespace papas
#endif
