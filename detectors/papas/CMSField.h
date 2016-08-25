//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef CMSFIELD_H
#define CMSFIELD_H

#include "Field.h"

namespace papas {

class VolumeCylinder;
/// CMS specific implementation of Detector Field element
///
class CMSField : public Field {
public:
  // TODO consider best approach for below?
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
   */  CMSField(const VolumeCylinder& volume, double magnitude);

private:
};

}  // end namespace papas
#endif
