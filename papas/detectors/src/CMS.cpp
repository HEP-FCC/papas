/**
 * @file CMS.cpp
 * @brief Implementation of the CMS detector
 */
#include "papas/detectors/CMS.h"

#include "papas/utility/PDebug.h"
#include "papas/datatypes/Track.h"
#include "papas/datatypes/Particle.h"
#include "papas/detectors/CMSEcal.h"
#include "papas/detectors/CMSField.h"
#include "papas/detectors/CMSHcal.h"
#include "papas/detectors/CMSTracker.h"

namespace papas {

  CMS::CMS(std::shared_ptr<const Calorimeter> ecal,
           std::shared_ptr<const Calorimeter> hcal,
           std::shared_ptr<const Tracker> tracker,
           std::shared_ptr<const Field> field,
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

  
double CMS::electronAcceptance(const Track& track) const {
  return track.p3().Mag() > m_electronAcceptanceMagnitude && fabs(track.p3().Eta()) < m_electronAcceptanceEta;
}

double CMS::electronEnergyResolution(const Particle& ptc) const {
  return m_electronEnergyFactor / sqrt(ptc.e());  //default factor = 0.1
}

double CMS::muonAcceptance(const Track& track) const {
  /// returns True if muon is seen.
  /// The CLIC CDR gives 99% for E > 7.5GeV and polar angle > 10 degrees
  ///
  return (track.p3().Mag() > m_muonAcceptanceMagnitude &&                  // default m_muonAcceptanceMagnitude=7.5
          fabs(track.theta()) < m_muonAcceptanceTheta * M_PI / 180.);  //default m_muonAcceptanceTheta 80
}

double CMS::muonResolution(const Particle& ptc) const {
  return m_muonResolution;  // default 0.02;
}



}  // end namespace papas
