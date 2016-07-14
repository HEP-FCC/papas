//
//  Created by Alice Robson on 09/11/15.
//
//
#include "detector.h"
#include "geometry.h"
#include "material.h"
#include <list>
#include <vector>

namespace papas {

DetectorElement::DetectorElement(papas::Layer layer, const VolumeCylinder& volume, const Material& material)
    : m_volume(volume), m_material(material), m_layer(layer) {}

Detector::Detector() {}

// aim to make this a const function and to do sort at initialisation
const std::list<SurfaceCylinder>& Detector::sortedCylinders() {
  // Return list of surface cylinders sorted by increasing radius.'''

  return m_cylinders;
  /* for element in self.elements.values():
   if element.volume.inner is not None:
   self._cylinders.append(element.volume.inner)
   self._cylinders.append(element.volume.outer)
   self._cylinders.sort(key=operator.attrgetter("rad"))
  return m_cylinders;*/
}

std::shared_ptr<const Calorimeter> Detector::calorimeter(papas::Layer layer) const {
  switch (layer) {
  case papas::Layer::kEcal:
    return m_ecal;
    break;
  case papas::Layer::kHcal:
    return m_hcal;
    break;
  default:
      throw std::range_error( "Cluster not found" );
  }
}

std::shared_ptr<const DetectorElement> Detector::element(Layer layer) const {

  switch (layer) {
  case papas::Layer::kEcal:
    return m_ecal;
    break;
  case papas::Layer::kHcal:
    return m_hcal;
    break;
  default:
      throw std::range_error( "Cluster not found" );
    // TODO add track and field
  }

  // TODO is this the best route or should it throw
  //return nullptr;
}

Field::Field(const VolumeCylinder& volume, const Material& material, double magnitude)
    : DetectorElement(Layer::kField, volume, material), m_magnitude(magnitude) {}

}  // end namespace papas