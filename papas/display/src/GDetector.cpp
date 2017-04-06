//
//  Created by Alice Robson on 09/11/15.
//
//
#include "papas/display/GDetector.h"
#include "papas/detectors/Detector.h"
#include "papas/detectors/Calorimeter.h"

namespace papas {

GDetector::GDetector(const Detector& detector) : m_gEcal(detector.ecal()), m_gHcal(detector.hcal()) {}

void GDetector::Draw(const std::string& projection) {
  m_gHcal.Draw(projection);
  m_gEcal.Draw(projection);
}

void GDetector::DrawSimple(const std::string& projection) const {
  auto gelem = GDetectorElement(3., 2);
  gelem.Draw(projection);
}

}  // end namespace papas
