//
//  Created by Alice Robson on 09/11/15.
//
//
#include "Detector.h"
#include "Material.h"
#include "SurfaceCylinder.h"
#include <list>
#include <vector>

namespace papas {

Detector::Detector() {}

std::shared_ptr<const Calorimeter> Detector::calorimeter(papas::Layer layer) const {
  switch (layer) {
  case papas::Layer::kEcal:
    return m_ecal;
    break;
  case papas::Layer::kHcal:
    return m_hcal;
    break;
  default:
    throw std::range_error("Detector Calorimeter not found");
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
    throw std::range_error("TODO: Detector Element Layer not found");
    // TODO add track and field
  }
}

}  // end namespace papas
