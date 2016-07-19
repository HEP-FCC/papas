//
//  Created by Alice Robson on 09/11/15.
//
//
#include "DetectorElement.h"
#include "VolumeCylinder.h"
#include "Material.h"
#include <list>
#include <vector>

namespace papas {

DetectorElement::DetectorElement(papas::Layer layer, const VolumeCylinder& volume, const Material& material)
    : m_volume(volume), m_material(material), m_layer(layer) {}

}  // end namespace papas
