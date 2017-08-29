#include "papas/detectors/cms/CMSField.h"

namespace papas {

CMSField::CMSField(double magnitude, double radius, double z)
    : Field(VolumeCylinder(Layer::kField, radius, z), Material("void", 0., 0.), magnitude) {}

}  // end namespace papas
