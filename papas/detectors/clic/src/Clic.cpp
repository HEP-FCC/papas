#include "papas/detectors/clic/Clic.h"

#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Track.h"
#include "papas/detectors/clic/ClicEcal.h"
#include "papas/detectors/clic/ClicField.h"
#include "papas/detectors/clic/ClicHcal.h"
#include "papas/detectors/clic/ClicTracker.h"

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

bool Clic::electronAcceptance(const Track& track) const {
  return track.p3().Mag() > m_electronAcceptanceMagnitude && fabs(track.p3().Eta()) < m_electronAcceptanceEta;
}

double Clic::electronEnergyResolution(const Particle& ptc) const {
  return m_electronEnergyFactor / sqrt(ptc.e());
}  // 0.1

bool Clic::muonAcceptance(const Track& track) const {
  /// returns True if muon is seen.
  /// The CLIC CDR gives 99% for E > 7.5GeV and polar angle > 10 degrees
  ///
  return (track.p3().Mag() > m_muonAcceptanceMagnitude &&              // 7.5
          fabs(track.theta()) < m_muonAcceptanceTheta * M_PI / 180.);  // 80
}

double Clic::muonResolution(const Particle& ptc) const {
  return m_muonResolution;  // 0.02;
}

}  // end namespace papas
