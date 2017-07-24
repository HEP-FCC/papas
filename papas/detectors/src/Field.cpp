#include "papas/detectors/Field.h"

namespace papas {

Field::Field(const VolumeCylinder& volume, const Material& material, double magnitude)
    : DetectorElement(Layer::kField, volume, material), m_magnitude(magnitude) {}
Field::Field(const VolumeCylinder&& volume, const Material&& material, double magnitude)
    : DetectorElement(Layer::kField, volume, material), m_magnitude(magnitude) {}

}  // end namespace papas
