/**
 * @file Clic.cc
 * @brief Implementation of the Clic detector
 */
#include "papas/detectors/ClicField.h"

namespace papas {

ClicField::ClicField(double magnitude, double radius, double z) :
  Field( VolumeCylinder(Layer::kField, radius, z), Material("void", 0, 0), magnitude) {}

}  // end namespace papas
