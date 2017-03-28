//
//  Created by Alice Robson on 09/11/15.
//
//
#include "papas/detectors/Detector.h"
#include "papas/detectors/Material.h"
#include "papas/detectors/SurfaceCylinder.h"
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
  case papas::Layer::kTracker:
    return m_tracker;
    break;
  case papas::Layer::kField:
    return m_field;
    break;
  default:
    throw std::range_error("TODO: Detector Element Layer not found");
  }
}

void Detector::setupElements() {
  for (auto layer : {papas::Layer::kTracker, papas::Layer::kEcal, papas::Layer::kHcal}) {
    std::shared_ptr<const DetectorElement> el = element(layer);
    m_elements.push_back(el);
  }
}
}  // end namespace papas
