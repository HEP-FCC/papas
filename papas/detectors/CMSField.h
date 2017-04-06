#ifndef CMSFIELD_H
#define CMSFIELD_H

#include "papas/detectors/Field.h"

namespace papas {

  //Forward declaration
class VolumeCylinder;
/// CMS specific implementation of Detector Field element
///
class CMSField : public Field {
public:
  /** Constructor
   *
   * @param[in] volume The field cyclinders
   * @param[in] magnitude field strength
   */
  CMSField(const VolumeCylinder&& volume, double magnitude);
  /** Constructor
   *
   * @param[in] volume the field cyclinders
   * @param[in] magnitude field strength
   */ CMSField(const VolumeCylinder& volume, double magnitude);

private:
};

}  // end namespace papas
#endif
