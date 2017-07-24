/**
 * @file CMS.cc
 * @brief Implementation of the CMS detector
 */
#include "papas/detectors/cms/CMSField.h"

namespace papas {

CMSField::CMSField(double magnitude, double radius, double x0, double lambdaI, double z) :
  Field( VolumeCylinder(Layer::kField, radius, z), Material("void", x0, lambdaI), magnitude) {}


}  // end namespace papas
