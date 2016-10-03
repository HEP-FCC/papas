//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @file CMS.cpp
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
                  1.479,                        // eta_crack
                  std::vector<double>{0.3, 1},  // emin barrel and endcap
                  std::vector<std::vector<double>>{{4.22163e-02, 1.55903e-01, 7.14166e-03},
                                                   {-2.08048e-01, 3.25097e-01, 7.34244e-03}},
                  std::vector<std::vector<double>>{{1.00071, -9.04973, -2.48554},
                                                   {9.95665e-01, -3.31774, -2.11123}})};  // barrel and endcap

  // HCAL detector element
  m_hcal = std::shared_ptr<const class Calorimeter>{
      new CMSHCAL(VolumeCylinder(Layer::kHcal, 2.9, 3.6, 1.9, 2.6),
                  Material(0.0, 0.17),
                  1.3,  // eta crack
                  std::vector<std::vector<double>>{{0.8062, 2.753, 0.1501}, {6.803e-06, 6.676, 0.1716}},
                  std::vector<std::vector<double>>{{1.036, 4.452, -2.458}, {1.071, 9.471, -2.823}})};
  // Tracker detector element
  m_tracker = std::shared_ptr<const Tracker>{new CMSTracker(VolumeCylinder(Layer::kTracker, 1.29, 1.99))};

  // Field detector element
  m_field = std::shared_ptr<const Field>{new CMSField(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8)};
}

}  // end namespace papas
