/**
 * @file CMS.cpp
 * @brief Implementation of the CMS detector
 */
#include "papas/detectors/CMS.h"

#include "papas/detectors/CMSEcal.h"
#include "papas/detectors/CMSField.h"
#include "papas/detectors/CMSHcal.h"
#include "papas/detectors/CMSTracker.h"

namespace papas {

CMS::CMS() : Detector() {
  // ECAL detector Element
  m_ecal = std::make_shared<const CMSECAL>(
      CMSECAL(VolumeCylinder(Layer::kEcal, 1.55, 2.1, 1.30, 2),
                  Material("CMS_ECAL", 8.9e-3, 0.275),
                  1.479,                        // eta_crack
                  std::vector<double>{0.3, 1},  // emin barrel and endcap
                  std::vector<std::vector<double>>{{4.22163e-02, 1.55903e-01, 7.14166e-03},
                                                   {-2.08048e-01, 3.25097e-01, 7.34244e-03}},
                  std::vector<std::vector<double>>{{1.00071, -9.04973, -2.48554},
                                                   {9.95665e-01, -3.31774, -2.11123}}));  // barrel and endcap

  // HCAL detector element
  m_hcal = std::make_shared<const CMSHCAL>(
      CMSHCAL(VolumeCylinder(Layer::kHcal, 2.9, 3.6, 1.9, 2.6),
                  Material("CMS_HCAL", 0.0, 0.17),
                  1.3,  // eta crack
                  std::vector<std::vector<double>>{{0.8062, 2.753, 0.1501}, {6.803e-06, 6.676, 0.1716}},
                  std::vector<std::vector<double>>{{1.036, 4.452, -2.458}, {1.071, 9.471, -2.823}}));
  // Tracker detector element
  m_tracker = std::make_shared<const CMSTracker>( CMSTracker(VolumeCylinder(Layer::kTracker, 1.29, 1.99)));

  // Field detector element
  m_field = std::make_shared<const CMSField>(CMSField(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8));
  setupElements();  // sets up a list of all detector elements (m_elements) (needed for propagation)
}

}  // end namespace papas
