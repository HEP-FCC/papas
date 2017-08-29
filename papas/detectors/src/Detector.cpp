#include "papas/detectors/Detector.h"

#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Track.h"
#include "papas/detectors/Calorimeter.h"
#include "papas/detectors/Field.h"
#include "papas/detectors/Tracker.h"

namespace papas {

Detector::Detector() {}

Detector::Detector(std::shared_ptr<const Calorimeter> ecal,
                   std::shared_ptr<const Calorimeter>
                       hcal,
                   std::shared_ptr<const Tracker>
                       tracker,
                   std::shared_ptr<const Field>
                       field)
    : m_ecal(ecal), m_hcal(hcal), m_tracker(tracker), m_field(field) {
  setupElements();
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
