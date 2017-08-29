#include "papas/detectors/clic/ClicField.h"

namespace papas {

ClicField::ClicField(double magnitude, double radius, double z)
    : Field(VolumeCylinder(Layer::kField, radius, z), Material("Clic_Field", 0, 0), magnitude) {}

}  // end namespace papas
