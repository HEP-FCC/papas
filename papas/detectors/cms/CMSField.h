#ifndef CMSFIELD_H
#define CMSFIELD_H

#include "papas/detectors/Field.h"

namespace papas {

/// CMS specific implementation of Detector Field element
///
class CMSField : public Field {
public:
  /** Constructor
   * @param[in] magnitude field strength
   * @param[in] radius field cylinder radius
   * @param[in] z field cylinder z
   * @param[in] x0 field material X0
   * @param[in] lambdaI field material lambdaI
   */
  CMSField(double magnitude = 3.8, double radius = 2.9, double z = 3.6);

private:
};

}  // end namespace papas
#endif
