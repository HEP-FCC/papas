#include "papas/detectors/DetectorElement.h"

namespace papas {

DetectorElement::DetectorElement(papas::Layer layer, const VolumeCylinder&& volume, const Material&& material)
    : m_volume(volume), m_material(material), m_layer(layer) {}
DetectorElement::DetectorElement(Layer layer, const VolumeCylinder& volume, const Material& material)
    : m_volume(volume), m_material(material), m_layer(layer) {}
}  // end namespace papas
