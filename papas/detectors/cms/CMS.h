#ifndef CMS_H
#define CMS_H

#include "papas/detectors/Detector.h"

namespace papas {

/// CMS specific implementation of Detector

/** Constructor
 *
 @param[in] ecal shared pointer to ECAL class
 @param[in] hcal shared pointer to HCAL class
 @param[in] tracker shared pointer to tracker class
 @param[in] field shared pointer to field class
 @param[in] electronAcceptanceMagnitude track energy must exceed this for electron acceptance
 @param[in] electronAcceptanceEta track eta must be less than this for electron acceptance
 @param[in] muonAcceptanceMagnitude track energy must exceed this for muon acceptance
 @param[in] muonAcceptanceTheta track energy must be less than this for muon acceptance
 @param[in] electronEnergyFactor governs energy resolution for electron
 @param[in] muonResolution energy resolution for muon
 */
class CMS : public Detector {
public:
  CMS(std::shared_ptr<const Calorimeter> ecal,
      std::shared_ptr<const Calorimeter>
          hcal,
      std::shared_ptr<const Tracker>
          tracker,
      std::shared_ptr<const Field>
          field,
      double electronAcceptanceMagnitude = 5.,
      double electronAcceptanceEta = 2.5,
      double muonAcceptanceMagnitude = 7.5,
      double muonAcceptanceTheta = 80,
      double electronEnergyFactor = 0.1,
      double muonResolution = 0.02);

  /**  Return whether or not an electron will be accepted
   @param[in] track track of the electron
   */
  bool electronAcceptance(const Track& track) const;

  /** Energy resolution for an electron
   @param[in] particle particle which should be an electron
   */
  double electronEnergyResolution(const Particle& ptc) const;

  /**  Return whether or not a muon will be accepted
   @param[in] track track of the muon
   */
  bool muonAcceptance(const Track& track) const;

  /** Energy resolution for a muon
   @param[in] particle particle which should be a muon
   */
  double muonResolution(const Particle& ptc) const;

private:
  double m_electronAcceptanceMagnitude;  ///< track energy must exceed this for electron acceptance
  double m_electronAcceptanceEta;        ///< track eta must be less than this for electron acceptance
  double m_muonAcceptanceMagnitude;      ///< track energy must exceed this for muon  acceptance
  double m_muonAcceptanceTheta;          ///< track energy must be less than this for muon acceptance
  double m_electronEnergyFactor;         ///< governs energy resolution electron
  double m_muonResolution;               ///< energy resolution muon
};

/* creates a default CMS detector
*/
CMS CreateDefaultCMS();

}  // end namespace papas
#endif
