#include "papas/detectors/clic/Clic.h"

#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Track.h"
#include "papas/detectors/clic/ClicEcal.h"
#include "papas/detectors/clic/ClicField.h"
#include "papas/detectors/clic/ClicHcal.h"
#include "papas/detectors/clic/ClicTracker.h"
#include "papas/utility/TRandom.h"

#include <cmath>

namespace papas {

Clic::Clic(std::shared_ptr<const Calorimeter> ecal,
           std::shared_ptr<const Calorimeter>
               hcal,
           std::shared_ptr<const Tracker>
               tracker,
           std::shared_ptr<const Field>
               field,
           double electronAcceptanceMagnitude,
           double electronAcceptanceEta,
           double electronAcceptanceEfficiency,
           double muonAcceptanceMagnitude,
           double muonAcceptanceTheta,
           double muonResolution)
    : Detector(ecal, hcal, tracker, field),
      m_electronAcceptanceMagnitude(electronAcceptanceMagnitude),
      m_electronAcceptanceEta(electronAcceptanceEta),
      m_electronAcceptanceEfficiency(electronAcceptanceEfficiency),
      m_muonAcceptanceMagnitude(muonAcceptanceMagnitude),
      m_muonAcceptanceTheta(muonAcceptanceTheta),
      m_muonResolution(muonResolution) {}

bool Clic::electronAcceptance(const Track& track) const {
  double thetaMax = std::dynamic_pointer_cast<const ClicTracker>(m_tracker)->thetaMax();
  if (track.p3().Pt() > m_electronAcceptanceMagnitude && fabs(track.theta()) < thetaMax)
    return rootrandom::Random::uniform(0, 1) > m_electronAcceptanceEfficiency;
  return false;
}

double Clic::electronEnergyResolution(const Particle& ptc) const {
  // The CLIC CDR does not give any value for the electron resolution.
  // We simply use the ECAL resolution.
  return m_ecal->energyResolution(ptc.e(), ptc.eta());
}

bool Clic::muonAcceptance(const Track& track) const {
  /// returns True if muon is seen.
  /// The CLIC CDR gives 99% for E > 7.5GeV and polar angle > 10 degrees
  ///
  return (track.p3().Mag() > m_muonAcceptanceMagnitude &&              // 7.5
          fabs(track.theta()) < m_muonAcceptanceTheta * M_PI / 180.);  // 80
}

double Clic::muonResolution(const Particle& ptc) const {
  return std::dynamic_pointer_cast<const ClicTracker>(m_tracker)->resolution(ptc);
}

Clic CreateDefaultClic() {
  // This function could be written more briefly as all parameters in the HCAL, ECAL etc have
  // default values. However this presentation allows all parameters to
  // be viewed at once, and thus could be used as and easy basis from which to make changes of selected parameters

  std::vector<double> emin{0.5, 0.5};
  std::vector<double> eresBarrel{0.167, 0.010, 0.011};
  auto ecal = std::make_shared<const ClicECAL>(2.15,   // innerRadius
                                               2.6,    //  innerZ
                                               .25,    //  depth
                                               .015,   // clusterSizePhoton
                                               0.045,  // clusterSize
                                               2.76,   // etaAcceptance
                                               emin,
                                               eresBarrel,
                                               23,  // nX0
                                               1,   // nLambdaI
                                               1.   // eResponse
                                               );

  std::vector<double> eresBarrelHCAL{0.6, 0., 0.025};
  auto hcal = std::make_shared<const ClicHCAL>(2.4,             // innerRadius
                                               2.85,            // innerZ
                                               4.8,             // outerRadius
                                               5.3,             // outerZ
                                               0.25,            // clusterSize
                                               eresBarrelHCAL,  // eresBarrel
                                               0.018,           // x0
                                               0.17,            // lambdaI
                                               1.,              // eResponse
                                               2.76);           // etaAcceptance

  std::map<int, std::pair<double, double>> resMap = {{90, {8.2e-2, 9.1e-2}},
                                                     {80, {8.2e-4, 9.1e-3}},
                                                     {30, {9.9e-5, 3.8e-3}},
                                                     {20, {3.9e-5, 1.6e-3}},
                                                     {10, {2e-5, 7.2e-4}}};

  auto tracker = std::make_shared<const ClicTracker>(2.14,    // outerRadius
                                                     2.6,     // outerZ
                                                     0,       // x0 = 0,
                                                     0,       // lambdaI = 0,
                                                     80,      // thetaParam = 80,
                                                     resMap,  // resmap
                                                     0.4,     // double ptThresholdLow = 0.4,
                                                     0.95,    // double ptProbabilityLow = 0.95,
                                                     2.,      // double ptThresholdHigh = 2.,
                                                     0.99);   // double ptProbabilityHigh = 0.99);

  auto field = std::make_shared<const ClicField>(2,     // field magnitude
                                                 3.5,   // outerRadius
                                                 4.8);  // material lambdaI

  return Clic(ecal, hcal, tracker, field,
              5.,     // double electronAcceptanceMagnitude
              2.5,    // double electronAcceptanceEta
              0.95,   // double electronAcceptanceEfficiency
              7.5,    // double muonAcceptanceMagnitude
              80,     // double muonAcceptanceTheta
              0.02);  // double muonResolution
}

}  // end namespace papas
