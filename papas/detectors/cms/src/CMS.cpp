/**
 * @file CMS.cpp
 * @brief Implementation of the CMS detector
 */
#include "papas/detectors/cms/CMS.h"

#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Track.h"
#include "papas/detectors/cms/CMSEcal.h"
#include "papas/detectors/cms/CMSField.h"
#include "papas/detectors/cms/CMSHcal.h"
#include "papas/detectors/cms/CMSTracker.h"
#include "papas/utility/PDebug.h"

namespace papas {

CMS::CMS(std::shared_ptr<const Calorimeter> ecal,
         std::shared_ptr<const Calorimeter>
             hcal,
         std::shared_ptr<const Tracker>
             tracker,
         std::shared_ptr<const Field>
             field,
         double electronAcceptanceMagnitude,
         double electronAcceptanceEta,
         double muonAcceptanceMagnitude,
         double muonAcceptanceTheta,
         double electronEnergyFactor,
         double muonResolution)
    : Detector(ecal, hcal, tracker, field),
      m_electronAcceptanceMagnitude(electronAcceptanceMagnitude),
      m_electronAcceptanceEta(electronAcceptanceEta),
      m_muonAcceptanceMagnitude(muonAcceptanceMagnitude),
      m_muonAcceptanceTheta(muonAcceptanceTheta),
      m_electronEnergyFactor(electronEnergyFactor),
      m_muonResolution(muonResolution) {}

bool CMS::electronAcceptance(const Track& track) const {
  return track.p3().Mag() > m_electronAcceptanceMagnitude && fabs(track.p3().Eta()) < m_electronAcceptanceEta;
}

double CMS::electronEnergyResolution(const Particle& ptc) const {
  return m_electronEnergyFactor / sqrt(ptc.e());  // default factor = 0.1
}

bool CMS::muonAcceptance(const Track& track) const {
  /// returns True if muon is seen.
  /// The CLIC CDR gives 99% for E > 7.5GeV and polar angle > 10 degrees
  ///
  return (track.p3().Mag() > m_muonAcceptanceMagnitude &&              // default m_muonAcceptanceMagnitude=7.5
          fabs(track.theta()) < m_muonAcceptanceTheta * M_PI / 180.);  // default m_muonAcceptanceTheta 80
}

double CMS::muonResolution(const Particle& ptc) const {
  return m_muonResolution;  // default 0.02;
}

CMS CreateDefaultCMS() {
  // This function could be written more briefly as all parameters in the HCAL, ECAL etc have
  // default values. However this presentation allows all parameters to
  // be viewed at once, and thus could be used as and easy basis from which to make changes of selected parameters

  std::vector<std::vector<double>> eresECAL = {{4.22163e-02, 1.55903e-01, 7.14166e-03},
                                               {-2.08048e-01, 3.25097e-01, 7.34244e-03}};
  std::vector<std::vector<double>> erespECAL = {{1.00071, -9.04973, -2.48554}, {9.95665e-01, -3.31774, -2.11123}};
  std::vector<double> emin{0.3, 1};
  auto ecal = std::make_shared<const CMSECAL>(1.3,     // innerRadius
                                              2,       // innerZ
                                              1.55,    // outerRadius
                                              2.1,     // outerZ
                                              8.9e-3,  // x0
                                              0.275,   // lambdaI
                                              0.04,    // clusterSizePhoton
                                              0.07,    // clusterSize
                                              1.479,   // etaCrack
                                              2.93,    // etaAcceptanceThreshold
                                              0.2,     // ptAcceptanceThreshold
                                              1.479,   // etaEndcapMin
                                              3.,      // etaEndcapMax
                                              emin,
                                              eresECAL,
                                              erespECAL);

  std::vector<std::vector<double>> eresHCAL = {{0.8062, 2.753, 0.1501}, {6.803e-06, 6.676, 0.1716}};
  std::vector<std::vector<double>> erespHCAL = {{1.036, 4.452, -2.458}, {1.071, 9.471, -2.823}};
  std::vector<double> acceptanceParameters = {1.,  // energy
                                              1.,           -1.9381,     -1.75330,    3.,       1.1,      10., 1.05634,
                                              -1.66943e-01, 1.05997e-02, 8.09522e-01, -9.90855, -5.30366, 5.,  7.};

  auto hcal = std::make_shared<const CMSHCAL>(1.9,   // innerRadius
                                              2.6,   // innerZ
                                              2.9,   // outerRadius
                                              3.6,   // outerZ
                                              0.2,   // clusterSize
                                              0.,    // x0
                                              0.17,  // lambdaI
                                              1.3,   // etaCrack
                                              eresHCAL,
                                              erespHCAL,
                                              acceptanceParameters);

  auto tracker = std::make_shared<const CMSTracker>(1.29,    // outerRadius= 1.29
                                                    1.99,    // outerZ  = 1.99
                                                    0,       // x0 = 0,
                                                    0,       // lambdaI = 0,
                                                    1.1e-2,  // resolution = 1.1e-2
                                                    0.5,     //  ptThreshold = 0.5
                                                    1.35,    // double etaThresholdLow = 1.35,
                                                    0.95,    // double ptProbabilityLow = 0.95,
                                                    2.5,     // double etaThresholdHigh = 2.5,
                                                    0.9);    // double ptProbabilityHigh = 0.9);

  auto field = std::make_shared<const CMSField>(3.8,  // field magnitude
                                                2.9,  // outerRadius
                                                3.6   // outerZ
                                                );

  return CMS(ecal, hcal, tracker, field,
             5.,     // double electronAcceptanceMagnitude
             2.5,    // double electronAcceptanceEta
             7.5,    // double muonAcceptanceMagnitude
             80,     // double muonAcceptanceTheta
             0.1,    // double electronEnergyFactor
             0.02);  // double double muonResolution
}

}  // end namespace papas
