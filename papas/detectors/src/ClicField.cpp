/**
 * @file Clic.cc
 * @brief Implementation of the Clic detector
 */
#include "papas/detectors/ClicField.h"

namespace papas {

ClicField::ClicField(double magnitude, double radius, double z,double x0, double lambdaI) :
  Field( VolumeCylinder(Layer::kField, radius, z), Material("void", x0, lambdaI), magnitude) {}

}  // end namespace papas
