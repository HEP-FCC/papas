//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @file CMS.cc
 * @brief Implementation of the CMS detector
 */
#include "CMS.h"
#include "CMSEcal.h"
#include "CMSField.h"
#include "CMSHcal.h"
#include "CMSTracker.h"

#include <vector>

namespace papas {

CMS::CMS() : Detector() {
  // ECAL detector Element
  m_ecal = std::shared_ptr<const class Calorimeter>{
      new CMSECAL(VolumeCylinder(Layer::kEcal, 1.55, 2.1, 1.30, 2),
                  Material(8.9e-3, 0.275),
                  1.5,                                                                       // eta_crack
                  std::vector<double>{0.3, 1},                                               // emin barrel and endcap
                  std::vector<std::vector<double>>{{.073, .1, .005}, {.213, .224, .005}})};  // barrel and endcap

  // HCAL detector element
  m_hcal = std::shared_ptr<const class Calorimeter>{
      new CMSHCAL(VolumeCylinder(Layer::kHcal, 2.9, 3.6, 1.9, 2.6),
                  Material(0.0, 0.17),
                  1.3,  // eta crack
                  std::vector<std::vector<double>>{{1.25829, 0., 0.175950}, {1.32242e-06, 6.99123, 2.70281e-01}},
                  std::vector<std::vector<double>>{{1.03430, 5.23646, -2.03400}, {1.06742, 9.41242, -2.75191}})};

  // Tracker detector element
  m_tracker = std::shared_ptr<const Tracker>{new CMSTracker(VolumeCylinder(Layer::kTracker, 1.29, 1.99))};

  // Field detector element
  m_field = std::shared_ptr<const Field>{new CMSField(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8)};
}

}  // end namespace papas
