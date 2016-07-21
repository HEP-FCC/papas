//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef CMSFIELD_H
#define CMSFIELD_H

#include "Field.h"

namespace papas {

class VolumeCylinder;
/**
 * @file CMS.h
 * @brief Implementation of CMS detector
 */
class CMSField : public Field {
public:
  // TODO consider best approach for below?
  /// constructor - allows the Material and Volume to be created on the fly
  CMSField(const VolumeCylinder&& volume, double magnitude);
  /// constructor - requires the Material and Volume to be already in existance
  CMSField(const VolumeCylinder& volume, double magnitude);

private:
};

}  // end namespace papas
#endif
