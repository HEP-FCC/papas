#include "papas/detectors/Detector.h"

#include "papas/detectors/Field.h"
#include "papas/detectors/Calorimeter.h"
#include "papas/detectors/Tracker.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Track.h"

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
  
  double Detector::electronAcceptance(const Track& track) const {
    return track.p3().Mag() > 5 && fabs(track.p3().Eta()) < 2.5; }
  
  double Detector::electronEnergyResolution(const Particle& ptc) const {
    return 0.1 / sqrt(ptc.e()); }
  
  double Detector::muonAcceptance(const Track& track) const {
    return track.p3().Perp() > 5. && fabs(track.p3().Eta()) < 2.5; }
}  // end namespace papas
