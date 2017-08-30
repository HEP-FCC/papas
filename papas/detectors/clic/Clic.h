#ifndef Clic_H
#define Clic_H

#include "papas/detectors/Detector.h"
#include "papas/detectors/VolumeCylinder.h"
#include "papas/detectors/clic/ClicField.h"

namespace papas {

/// Clic specific implementation of Detector
class Clic : public Detector {
public:
  /** Constructor
   @param[in] ecal Clic Ecal
   @param[in] hcal Clic Hcal
   @param[in] tracker Clic Tracker
   @param[in] field Clic Field
   @param[in] electronAcceptanceMagnitude minimum energy for electron acceptance
   @param[in] electronAcceptanceEta maximum eta for electron acceptance
   @param[in] muonAcceptanceMagnitude minimum energy for muon  acceptance
   @param[in] muonAcceptanceTheta maximum theta for muon acceptance
   @param[in] electronEnergyFactor electron energy resolution parameter
   @param[in] muonResolution Muon resolution
   */
  Clic(std::shared_ptr<const Calorimeter> ecal,
       std::shared_ptr<const Calorimeter>
           hcal,
       std::shared_ptr<const Tracker>
           tracker,
       std::shared_ptr<const Field>
           field,
       double electronAcceptanceMagnitude = 5.,
       double electronAcceptanceEta = 2.5,
       double electronAcceptanceEfficiency = 0.95,
       double muonAcceptanceMagnitude = 7.5,
       double muonAcceptanceTheta = 80,
       double muonResolution = 0.02);

  /** Return whether or not an electron will be accepted
      @param[in] track track of the electron */
  bool electronAcceptance(const Track& track) const;

  /** Energy resolution for an electron
      @param[in] particle particle which should be an electron */
  double electronEnergyResolution(const Particle& ptc) const;

  /**  Return whether or not a muon will be accepted
      @param[in] track track of the muon */
  bool muonAcceptance(const Track& track) const;

  /** Energy resolution for a muon
      @param[in] particle particle which should be a muon */
  double muonResolution(const Particle& ptc) const;

private:
  double m_electronAcceptanceMagnitude;  ///< minimum energy for electron acceptance
  double m_electronAcceptanceEta;        ///< maximum eta for electron acceptance
  double m_electronAcceptanceEfficiency; ///< efficiency for electron acceptance
  double m_muonAcceptanceMagnitude;      ///< minimum energy for muon acceptance
  double m_muonAcceptanceTheta;          ///< maximum theta for muon acceptance
  double m_muonResolution;               ///< muon resolution
};
/* creates a default Clic detector
 */
Clic CreateDefaultClic();

}  // end namespace papas
#endif
