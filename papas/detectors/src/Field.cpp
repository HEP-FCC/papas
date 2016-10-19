//
//  Created by Alice Robson on 09/11/15.
//
//
#include "papas/detectors/Field.h"
#include "papas/detectors/Material.h"
#include "papas/detectors/VolumeCylinder.h"
#include <list>
#include <vector>

namespace papas {

Field::Field(const VolumeCylinder& volume, const Material& material, double magnitude)
    : DetectorElement(Layer::kField, volume, material), m_magnitude(magnitude) {}

}  // end namespace papas
