//
//  Created by Alice Robson on 09/11/15.
//
//
#include "Field.h"
#include "Material.h"
#include "VolumeCylinder.h"
#include <list>
#include <vector>

namespace papas {

Field::Field(const VolumeCylinder& volume, const Material& material, double magnitude)
    : DetectorElement(Layer::kField, volume, material), m_magnitude(magnitude) {}

}  // end namespace papas
